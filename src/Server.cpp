/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for Mac OSX written in C++
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
  static const size_t bufferSize = 256;
  static uint8_t buffer[bufferSize];
  
  Server::Server(string name, unsigned port, uint8_t maxClients)
  {
    if(name.length() > 20)
      throw WicError(WIC_ERRNO_LARGE_NAME);
    if(port < 1025)
      throw WicError(WIC_ERRNO_RESERVED_PORT);
    if(maxClients < 1)
      throw WicError(WIC_ERRNO_SMALL_MAX_CLIENTS);
    if(maxClients > 254)
      throw WicError(WIC_ERRNO_LARGE_MAX_CLIENTS);
    
    name_ = name;
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_ == -1)
      throw WicError(WIC_ERRNO_SOCKET_FAIL);
    fcntl(socket_, F_SETFL, O_NONBLOCK);
    bzero(&addr_, lenAddr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(port);
    int result = bind(socket_, (struct sockaddr*) &addr_, lenAddr_);
    if(result == -1)
    {
      close(socket_);
      if(errno == EADDRINUSE)
        throw WicError(WIC_ERRNO_PORT_IN_USE);
      else
        throw WicError(WIC_ERRNO_SOCKET_BIND_FAIL);
    }
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
  void Server::send(Packet& packet, NodeID destID)
  {
    if(destID >= maxNodes_)
      throw WicError(WIC_ERRNO_IMPOSSIBLE_INDEX);
    
    if(used_[destID])
    {
      size_t size = Packet::HEADER_SIZE + packet.size();
      packet.toBuffer(buffer);
      sendto(socket_, buffer, size, 0,
             (struct sockaddr*) &addrs_[destID], lenAddr_);
    }
    else
      throw WicError(WIC_ERRNO_INDEX_UNUSED);
  }
  void Server::sendExclude(Packet &packet, NodeID excludeID)
  {
    if(excludeID < 1)
      throw WicError(WIC_ERRNO_NOT_CLIENT_INDEX);
    if(excludeID >= maxNodes_)
      throw WicError(WIC_ERRNO_IMPOSSIBLE_INDEX);
    
    for(NodeID i = 1; i < maxNodes_; i++)
    {
      if(i != excludeID && used_[i])
        send(packet, i);
    }
  }
  void Server::sendAll(Packet& packet)
  {
    for(NodeID i = 1; i < maxNodes_; i++)
    {
      if(used_[i])
        send(packet, i);
    }
  }
  void Server::recv(Packet& result)
  {
    
    struct sockaddr_in recv_addr;
    socklen_t len_recv_addr = sizeof(recv_addr);
    ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                              (struct sockaddr*) &recv_addr, &len_recv_addr);
    if(length > 0)
    {
      NodeID sourceID;
      result.fromBuffer(buffer);
      if(result.type() == JoinRequest::type)
      {
        JoinRequest request = JoinRequest(result);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &recv_addr.sin_addr, &ip[0], INET_ADDRSTRLEN);
        for(unsigned i = 0; i < blacklist_.size(); i++)
        {
          if(blacklist_[i] == string(ip) || blacklist_[i] == request.name())
          {
            JoinResponse response = JoinResponse(JoinResponse::BANNED,
                                                 maxNodes_, 0, name_);
            size_t size = Packet::HEADER_SIZE + JoinResponse::size;
            response.toBuffer(buffer);
            sendto(socket_, buffer, size, 0, (struct sockaddr*) &recv_addr,
                   lenAddr_);
          }
        }
        uint8_t connections = 0;
        for(NodeID i = 1; i < maxNodes_; i++)
        {
          if(used_[i])
            connections++;
        }
        if(connections == maxNodes_ - 1)
        {
          JoinResponse response = JoinResponse(JoinResponse::FULL,
                                               maxNodes_, 0, name_);
          size_t size = Packet::HEADER_SIZE + JoinResponse::size;
          response.toBuffer(buffer);
          sendto(socket_, buffer, size, 0, (struct sockaddr*) &recv_addr,
                 lenAddr_);
        }
        NodeID newID;
        for(NodeID i = 0; i < maxNodes_; i++)
        {
          if(!used_[i])
          {
            sourceID = i;
            newID = i;
            break;
          }
        }
        
        JoinResponse response = JoinResponse(JoinResponse::OK,
                                             maxNodes_, newID, name_);
        addrs_[newID] = recv_addr;
        used_[newID] = true;
        names_[newID] = request.name();
        char tmp[20];
        inet_ntop(AF_INET, &recv_addr.sin_addr, tmp,INET_ADDRSTRLEN);
        ips_[newID] = string(tmp);
        send(response, newID);
        
        ClientJoined pkt = ClientJoined(newID, request.name());
        sendExclude(pkt, newID);
        result = pkt;
        for(NodeID i = 1; i < maxNodes_; i++)
        {
          if(used_[i])
          {
            ClientInfo pkt = ClientInfo(i, names_[i]);
            send(pkt, newID);
          }
        }
      }
      sourceID = result.source_;
      if(sourceID > 0 && sourceID < maxNodes_ && used_[sourceID] &&
         recv_addr.sin_addr.s_addr == addrs_[sourceID].sin_addr.s_addr &&
         recv_addr.sin_port == addrs_[sourceID].sin_port)
      {
        if(result.type() == Leaving::type)
        {
          Packet pkt = ClientLeft(sourceID, ClientLeft::NORMAL, "");
          sendExclude(pkt, sourceID);
          used_[sourceID] = false;
        }
      }
      throw WicError(WIC_ERRNO_PACKET_UNKNOWN_SOURCE);
    }
  }
  void Server::kick(NodeID ID, string reason)
  {
    if(ID < 1)
      throw WicError(WIC_ERRNO_NOT_CLIENT_INDEX);
    if(ID >= maxNodes_)
      throw WicError(WIC_ERRNO_IMPOSSIBLE_INDEX);
    if(!used_[ID])
      throw WicError(WIC_ERRNO_INDEX_UNUSED);
    if(reason.length() > 50)
      throw WicError(WIC_ERRNO_LARGE_REASON);
    
    Packet pkt = Kick(reason);
    send(pkt, ID);
    pkt = ClientLeft(ID, ClientLeft::KICKED, reason);
    sendExclude(pkt, ID);
    used_[ID] = false;
  }
  void Server::kick(string nameOrIP, string reason)
  {
    kick(getID(nameOrIP), reason);
  }
  void Server::ban(NodeID ID)
  {
    if(ID < 1)
      throw WicError(WIC_ERRNO_NOT_CLIENT_INDEX);
    if(ID >= maxNodes_)
      throw WicError(WIC_ERRNO_IMPOSSIBLE_INDEX);
    if(!used_[ID])
      throw WicError(WIC_ERRNO_INDEX_UNUSED);
    
    blacklist_.push_back(names_[ID]);
    Packet pkt = Ban("");
    send(pkt, ID);
    pkt = ClientLeft(ID, ClientLeft::BANNED, "");
    sendExclude(pkt, ID);
    used_[ID] = false;
  }
  
  
  
  void Server::ban(string nameOrIP)
  {
    if(nameOrIP.length() > 20)
      throw WicError(WIC_ERRNO_LARGE_NAME_OR_IP);
    blacklist_.push_back(nameOrIP);
  }
  void Server::unban(string nameOrIP)
  {
    if(nameOrIP.length() > 20)
      throw WicError(WIC_ERRNO_LARGE_NAME_OR_IP);
    
    for(unsigned i = 0; i < blacklist_.size(); i++)
    {
      if(nameOrIP == blacklist_[i])
      {
        blacklist_.erase(blacklist_.begin() + i);
        i--;
      }
    }
    throw WicError(WIC_ERRNO_UNBANNED_NAME_OR_IP);
  }
  NodeID Server::getID(string nameOrIP)
  {
    if(nameOrIP.length() > 20)
      throw WicError(WIC_ERRNO_LARGE_NAME_OR_IP);
    
    for(unsigned i = 0; i < maxNodes_; i++)
    {
      if(used_[i] && (nameOrIP == names_[i] || nameOrIP == ips_[i]))
        return i;
    }
    throw WicError(WIC_ERRNO_NO_SUCH_CLIENT);
  }
}
