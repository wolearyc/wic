/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for MacOS written in C++
 * Copyright (C) 2013-2017  Willis O'Leary
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 * File:    Server.cpp
 * ----------------------------------------------------------------------------
 */
#include "Server.h"
namespace wic
{
  const size_t bufferSize = 255;
  uint8_t buffer[255];

  Server::Server(string name, unsigned port, uint8_t maxClients)
  : Node(name, port)
  {
    if(name.length() > 20)
      throw InvalidArgument("name", "should be less than 20 characters long");
    if(port < 1025)
      throw InvalidArgument("port", "should be > 1024");
    if(maxClients < 1)
      throw InvalidArgument("maxClients", "should be nonzero");
    if(maxClients > 254)
      throw InvalidArgument("maxClients", "should be < 255");
    
    ID_ = 0;
    name_ = name;
    
    maxNodes_ = 1 + maxClients;
    addrs_.reserve(maxNodes_);
    addrs_[0] = addr_;
    used_.reserve(maxNodes_);
    used_[0] = true;
    names_.reserve(maxNodes_);
    names_[0] = name_;
    ips_.reserve(maxNodes_);
    char tmp[20];
    inet_ntop(AF_INET, &addr_.sin_addr, tmp, INET_ADDRSTRLEN);
    ips_[0] = string(tmp);
  }
  Server::~Server()
  {
    Shutdown pkt = Shutdown();
    sendAll(pkt);
    close(socket_);
  }
  void Server::send(const AbstractPacket& packet, NodeID destID) const
  {
    if(destID == 0)
      throw InvalidArgument("destID", "should be nonzero");
    if(destID >= maxNodes_)
      throw InvalidArgument("destID", "destination ID is impossible");
    
    if(used_[destID])
    {
      size_t size = AbstractPacket::HEADER_SIZE + packet.size();
      packet.toBuffer(buffer, packet.source());
      // Server doesn't mess with the source
      sendto(socket_, buffer, size, 0,
             (struct sockaddr*) &addrs_[destID], lenAddr_);
    }
    else
      throw Error("destination ID corresponds with no client");
  }
  void Server::sendExclude(const AbstractPacket &packet, NodeID excludeID) const
  {
    if(excludeID  == 0)
      throw InvalidArgument("excludeID", "should be nonzero");
    if(excludeID >= maxNodes_)
      throw InvalidArgument("excludeID", "should be less than max clients");
    
    for(NodeID i = 1; i < maxNodes_; i++)
    {
      if(i != excludeID && used_[i])
        send(packet, i);
    }
  }
  void Server::sendAll(const AbstractPacket& packet) const
  {
    for(NodeID i = 1; i < maxNodes_; i++)
    {
      if(used_[i])
        send(packet, i);
    }
  }
  bool Server::recv(MysteryPacket& result)
  {
    struct sockaddr_in recvAddr;
    socklen_t tmpLen = sizeof(recvAddr);
    ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                              (struct sockaddr*) &recvAddr, &tmpLen);
    if(length > 0)
    {
      NodeID sourceID;
      result.populate(buffer);
      
      // Recieved packet is a join request, so process and return
      if(result.isType<JoinRequest>())
      {
        JoinRequest joinRequest(result);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &recvAddr.sin_addr, &ip[0], INET_ADDRSTRLEN);
        
        // Search the blacklist. If banned, respond and return.
        for(unsigned i = 0; i < blacklist_.size(); i++)
        {
          if(blacklist_[i] == string(ip) || blacklist_[i] == joinRequest.name())
          {
            JoinResponse joinResponse(JoinResponse::BANNED, maxNodes_, 0, name_);
            size_t size = AbstractPacket::HEADER_SIZE + joinResponse.size();
            joinResponse.toBuffer(buffer, ID());
            sendto(socket_, buffer, size, 0, (struct sockaddr*) &recvAddr,
                   lenAddr_);
            return true;
          }
        }
        
        // Determine connections. If full, respond and return
        uint8_t connections = 0;
        for(NodeID i = 1; i < maxNodes_; i++)
        {
          if(used_[i])
            connections++;
        }
        if(connections == maxNodes_ - 1)
        {
          JoinResponse joinResponse(JoinResponse::FULL, maxNodes_, 0, name_);
          size_t size = AbstractPacket::HEADER_SIZE + joinResponse.size();
          joinResponse.toBuffer(buffer, ID());
          sendto(socket_, buffer, size, 0, (struct sockaddr*) &recvAddr,
                 lenAddr_);
          return true;
        }
        
        // Join ok. Find a new ID and repond
        NodeID newID;
        for(NodeID i = 0; i < maxNodes_; i++)
        {
          if(!used_[i])
          {
            sourceID = i;
            newID = i;
          }
        }
        JoinResponse joinResponse(JoinResponse::OK, maxNodes_, newID, name_);
        send(joinResponse, newID);
        
        // Setup new connection
        addrs_[newID] = recvAddr;
        used_[newID] = true;
        names_[newID] = joinRequest.name();
        char tmp[20];
        inet_ntop(AF_INET, &recvAddr.sin_addr, tmp,INET_ADDRSTRLEN);
        ips_[newID] = string(tmp);
        
        // Notify new client of old clients
        ClientJoined clientJoined(newID, joinRequest.name());
        sendExclude(clientJoined, newID);
        result.populate(clientJoined);
        for(NodeID i = 1; i < maxNodes_; i++)
        {
          if(used_[i])
          {
            ClientInfo clientInfo(i, names_[i]);
            send(clientInfo, newID);
          }
        }
        return true;
      }
      
      // Other type of packet; verify source
      sourceID = result.source();
      if(sourceID > 0 && sourceID < maxNodes_ && used_[sourceID] &&
         recvAddr.sin_addr.s_addr == addrs_[sourceID].sin_addr.s_addr &&
         recvAddr.sin_port == addrs_[sourceID].sin_port)
      {
        // Client left. Notify all clients of exit.
        if(result.isType<Leaving>())
        {
          ClientLeft clientLeft(sourceID, ClientLeft::NORMAL, "");
          sendExclude(clientLeft, sourceID);
          used_[sourceID] = false;
        }
        return true;
      }
      throw Error("packet recieved from unknown source");
    }
    return false;
  }
  void Server::kick(NodeID ID, string reason)
  {
    if(ID < 1)
      throw InvalidArgument("ID", "should be nonzero");
    if(ID >= maxNodes_)
      throw InvalidArgument("ID", "should be less than max clients");
    if(!used_[ID])
      throw Error("ID corresponds with no client");
    if(reason.length() > 50)
      throw InvalidArgument("reason", "should be under 51 characters");
    
    Kick kickPkt(reason);
    send(kickPkt, ID);
    ClientLeft clientLeft(ID, ClientLeft::KICKED, reason);
    sendExclude(clientLeft, ID);
    used_[ID] = false;
  }
  void Server::kick(string nameOrIP, string reason)
  {
    kick(getID(nameOrIP), reason);
  }
  void Server::ban(NodeID ID)
  {
    if(ID < 1)
      throw InvalidArgument("ID", "should be nonzero");
    if(ID >= maxNodes_)
      throw InvalidArgument("ID", "should be less than max clients");
    if(!used_[ID])
      throw Error("ID corresponds with no client");
    
    blacklist_.push_back(names_[ID]);
    Ban banPkt("");
    send(banPkt, ID);
    ClientLeft clientLeft(ID, ClientLeft::BANNED, "");
    sendExclude(clientLeft, ID);
    used_[ID] = false;
  }

  void Server::ban(string nameOrIP)
  {
    blacklist_.push_back(nameOrIP);
  }
  void Server::unban(string nameOrIP)
  {
    for(unsigned i = 0; i < blacklist_.size(); i++)
    {
      if(nameOrIP == blacklist_[i])
      {
        blacklist_.erase(blacklist_.begin() + i);
        i--;
      }
    }
    throw Error("no blacklist entry for nameOrIP");
  }
  NodeID Server::getID(string nameOrIP) const
  {

    for(unsigned i = 0; i < maxNodes_; i++)
    {
      if(used_[i] && (nameOrIP == names_[i] || nameOrIP == ips_[i]))
        return i;
    }
    throw Error("nameOrIP corresponds to no client");
  }
}
