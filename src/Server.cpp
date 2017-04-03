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
  // Utility buffer.
  const size_t bufferSize = 255;
  uint8_t buffer[bufferSize];

  Server::Server(string name, unsigned port, uint8_t maxClients)
  : Node(name, port)
  {
    if(maxClients == 0)
      throw InvalidArgument("maxClients", "zero");
    if(maxClients > 254)
      throw InvalidArgument("maxClients", "> 254");
    
    joined = true;
    ID = 0;
    maxID = maxClients;
    addrs.resize(getMaxNodes());
    addrs[0] = addr;
    used.resize(getMaxNodes());
    used[0] = true;
    names.resize(getMaxNodes());
    names[0] = name;
    ips.resize(getMaxNodes());
    char tmp[20];
    inet_ntop(AF_INET, &addr.sin_addr, tmp, INET_ADDRSTRLEN);
    ips[0] = string(tmp);
  }
  Server::~Server()
  {
    sendAll(Shutdown());
    close(sock);
  }
  void Server::send(const AbstractPacket& packet, NodeID destID) const
  {
    if(destID == 0)
      throw InvalidArgument("destID", "zero");
    if(destID > getMaxID())
      throw InvalidArgument("destID", "> maxID");
    if(!isUsed(destID))
      throw InvalidArgument("destID", "unused");

    size_t size = AbstractPacket::HEADER_SIZE + packet.getSize();
    packet.toBuffer(buffer, packet.getSource());
    // Server doesn't mess with the source
    sendto(sock, buffer, size, 0, (struct sockaddr*) &addrs[destID], lenAddr);
  }
  void Server::sendExclude(const AbstractPacket &packet, NodeID excludeID) const
  {
    if(excludeID  == 0)
      throw InvalidArgument("excludeID", "zero");
    if(excludeID > getMaxID())
      throw InvalidArgument("excludeID", "maxID");
    if(!isUsed(excludeID))
      throw InvalidArgument("destID", "unused");
    
    for(NodeID i = 1; i <= maxID; i++)
    {
      if(i != excludeID && used[i])
        send(packet, i);
    }
  }
  void Server::sendAll(const AbstractPacket& packet) const
  {
    for(NodeID i = 1; i <= maxID; i++)
    {
      if(used[i])
        send(packet, i);
    }
  }
  bool Server::recv(MysteryPacket& result)
  {
    struct sockaddr_in recvAddr;
    socklen_t tmpLen = sizeof(recvAddr);
    ssize_t length = recvfrom(sock, buffer, bufferSize, 0,
                              (struct sockaddr*) &recvAddr, &tmpLen);
    if(length > 0 && tmpLen == lenAddr)
    {
      result.populate(buffer);
      
      // Recieved packet is a join request, so process and return
      if(result.isType<JoinRequest>())
      {
        JoinRequest joinRequest(result);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &recvAddr.sin_addr, &ip[0], INET_ADDRSTRLEN);
        
        // Search the blacklist. If banned, respond and return.
        for(unsigned i = 0; i < blacklist.size(); i++)
        {
          if(blacklist[i] == string(ip) || blacklist[i] == joinRequest.name())
          {
            JoinResponse joinResponse(JoinResponse::BANNED, maxID, 0, name);
            size_t size = AbstractPacket::HEADER_SIZE + joinResponse.getSize();
            joinResponse.toBuffer(buffer, Node::getID());
            sendto(sock, buffer, size, 0, (struct sockaddr*) &recvAddr,
                   lenAddr);
            return true;
          }
        }
        
        // Determine connections. If full, respond and return
        uint8_t nodes = 0;
        for(NodeID i = 0; i <= maxID; i++)
        {
          if(isUsed(i))
            nodes++;
        }
        if(nodes == getMaxNodes())
        {
          JoinResponse joinResponse(JoinResponse::FULL, maxID, 0, name);
          size_t size = AbstractPacket::HEADER_SIZE + joinResponse.getSize();
          joinResponse.toBuffer(buffer, getID());
          sendto(sock, buffer, size, 0, (struct sockaddr*) &recvAddr, lenAddr);
          return true;
        }
        
        // Join ok. Find a new ID, set up new connection, and respond.
        NodeID newID;
        for(NodeID i = 1; i <= maxID; i++)
        {
          if(!used[i])
          {
            newID = i;
            break;
          }
        }
        
        // Set up new connection
        addrs[newID] = recvAddr;
        used[newID] = true;
        names[newID] = joinRequest.name();
        char tmp[20];
        inet_ntop(AF_INET, &recvAddr.sin_addr, tmp,INET_ADDRSTRLEN);
        ips[newID] = string(tmp);
        
        JoinResponse joinResponse(JoinResponse::OK, getMaxID(), newID,
                                  getName());
        send(joinResponse, newID);
        
        // Bring all clients up to speed.
        ClientJoined clientJoined(newID, joinRequest.name());
        sendExclude(clientJoined, newID);
        result.populate(clientJoined);
        for(NodeID i = 1; i <= maxID; i++)
        {
          if(used[i])
          {
            ClientInfo clientInfo(i, names[i]);
            send(clientInfo, newID);
          }
        }
        return true;
      }
      
      // Other type of packet; verify source
      NodeID sourceID = result.getSource();
      if(sourceID > 0 && sourceID <= maxID && used[sourceID] &&
         recvAddr.sin_addr.s_addr == addrs[sourceID].sin_addr.s_addr &&
         recvAddr.sin_port == addrs[sourceID].sin_port)
      {
        // Client left. Notify all clients of exit.
        if(result.isType<Leaving>())
        {
          ClientLeft clientLeft(sourceID, ClientLeft::NORMAL, "");
          sendExclude(clientLeft, sourceID);
          used[sourceID] = false;
        }
        return true;
      }
      throw Failure("packet recieved from unknown source");
    }
    return false;
  }
  void Server::kick(NodeID ID, string reason)
  {
    if(ID == 0)
      throw InvalidArgument("ID", "zero");
    if(ID > getMaxID())
      throw InvalidArgument("ID", "maxID");
    if(!isUsed(ID))
      throw InvalidArgument("ID", "unused");
    if(reason.length() > 50)
      throw InvalidArgument("reason length", "> 50");
    
    send(Kick(reason), ID);
    sendExclude(ClientLeft(ID, ClientLeft::KICKED, reason), ID);
    used[ID] = false;
  }
  void Server::kick(string nameOrIP, string reason)
  {
    kick(getNodeID(nameOrIP), reason);
  }
  void Server::ban(NodeID ID)
  {
    if(ID < 1)
      throw InvalidArgument("ID", "zero");
    if(ID > getMaxID())
      throw InvalidArgument("ID", "maxID");
    if(!used[ID])
      throw InvalidArgument("ID", "unused");
    
    blacklist.push_back(names[ID]);
    send(Ban(""), ID);
    sendExclude(ClientLeft(ID, ClientLeft::BANNED, ""), ID);
    used[ID] = false;
  }
  void Server::ban(string nameOrIP)
  {
    try
    {
      ban(getNodeID(nameOrIP));
    }
    catch (Error error)
    {
      blacklist.push_back(nameOrIP);
    }
  }
  void Server::unban(string nameOrIP)
  {
    bool found = false;
    for(unsigned i = 0; i < blacklist.size(); i++)
    {
      if(nameOrIP == blacklist[i])
      {
        blacklist.erase(blacklist.begin() + i);
        i--;
        found = true;
      }
    }
    if(!found)
      throw Error("nameOrIP is not banned");
  }
  NodeID Server::getNodeID(string nameOrIP) const
  {

    for(unsigned i = 0; i <= maxID; i++)
    {
      if(used[i] && (nameOrIP == names[i] || nameOrIP == ips[i]))
        return i;
    }
    throw Error("nameOrIP corresponds to no client");
  }
}
