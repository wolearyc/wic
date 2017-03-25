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
 * File:    Client.cpp
 * ----------------------------------------------------------------------------
 */
#include "Client.h"
namespace wic
{
  static const size_t bufferSize = 256; // utility buffer size
  static uint8_t buffer[bufferSize];    // utility buffer
  Client::Client(string name)
  : Client()
  {
    if(name.length() > 20)
      throw WicError(WIC_ERRNO_LARGE_NAME);
    name_ = name;
  }
  Client::Client()
  : joined_(false), ID_(0), name_(""), maxNodes_(0), socket_(0),
    lenAddr_(sizeof(sockaddr_in))
  {
    bzero(&addr_, sizeof(addr_));
    bzero(&serverAddr_, sizeof(serverAddr_));
  }
  Client::~Client()
  {
    close(socket_);
  }
  Packet Client::join(unsigned port, string ip, double timeout)
  {
    if(port < 1025)
      throw WicError(WIC_ERRNO_RESERVED_PORT);
    if(ip.length() < 7)
      throw WicError(WIC_ERRNO_SMALL_LEN_SERVER_IP);
    if(joined_)
      throw WicError(WIC_ERRNO_CLIENT_ALREADY_JOINED);
    
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_ == -1)
      throw WicError(WIC_ERRNO_SOCKET_FAIL);
    fcntl(socket_, F_SETFL, O_NONBLOCK);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(0);
    int result = bind(socket_, (struct sockaddr*) &addr_, sizeof(addr_));
    if(result == -1)
    {
      close(socket_);
      if(errno == EADDRINUSE)
        throw WicError(WIC_ERRNO_PORT_IN_USE);
      else
        throw WicError(WIC_ERRNO_SOCKET_BIND_FAIL);
    }
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = inet_addr(ip.data());
    serverAddr_.sin_port = htons(port);
    
    Packet packet = JoinRequest(name_);
    send(packet);
    
    clock_t initial_clock = clock();
    while((clock() - initial_clock)/CLOCKS_PER_SEC <= timeout)
    {
      struct sockaddr_in recv_addr;
      socklen_t tmp_len = sizeof(recv_addr);
      ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                                (struct sockaddr*) &recv_addr, &tmp_len);
      if(length > 0)
      {
        packet = Packet(buffer);
        if(packet.type() == JoinResponse::type)
        {
          JoinResponse response = JoinResponse(packet);
          if(response.joined())
          {
            maxNodes_ = response.numNodes();
            used_ = vector<bool>(maxNodes_, false);
            used_[0] = true;
            names_ = vector<string>(maxNodes_, "");
            serverAddr_ = recv_addr;
            joined_ = true;
            ID_ = response.assignedID();
            used_[ID_] = true;
            names_[0] = response.serverName();
            names_[ID_] = name_;
            return response;
          }
          else if(response.full())
            throw WicError(WIC_ERRNO_JOIN_FAIL_FULL);
          else if(response.banned())
            throw WicError(WIC_ERRNO_JOIN_FAIL_BANNED);
        }
      }
    }
    throw WicError(WIC_ERRNO_TIMEOUT);
  }
  
  void Client::send(Packet& packet)
  {
    packet.source_ = ID_;
    size_t size = Packet::HEADER_SIZE + packet.size();
    packet.toBuffer(buffer);
    sendto(socket_, buffer, size, 0, (struct sockaddr*) &serverAddr_, lenAddr_);
  }
  void Client::recv(Packet& result)
  {
    if(!joined_)
      throw WicError(WIC_ERRNO_CLIENT_NOT_JOINED);
    
    struct sockaddr_in recv_addr;
    socklen_t tmp_len = sizeof(recv_addr);
    ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                              (struct sockaddr*) &recv_addr, &tmp_len);
    if(length > 0)
    {
      if(recv_addr.sin_addr.s_addr == serverAddr_.sin_addr.s_addr &&
         recv_addr.sin_port == serverAddr_.sin_port)
      {
        result.fromBuffer(buffer);
        if(result.type() == ClientJoined::type)
        {
          ClientJoined tmp = ClientJoined(result);
          used_[tmp.newID()] = true;
          names_[tmp.newID()] = tmp.newName();
        }
        else if(result.type() == ClientInfo::type)
        {
          ClientInfo tmp = ClientInfo(result);
          used_[tmp.ID()] = true;
          names_[tmp.ID()] = tmp.name();
        }
        else if(result.type() == Kick::type ||
                result.type() == Ban::type  ||
                result.type() == Shutdown::type)
        {
          joined_ = false;
        }
        else if(result.type() == ClientLeft::type)
        {
          ClientLeft tmp = ClientLeft(result);
          used_[tmp.oldID()] = false;
        }
      }
      throw WicError(WIC_ERRNO_PACKET_UNKNOWN_SOURCE);
    }
  }
  void Client::leave()
  {
    if(!joined_)
      throw WicError(WIC_ERRNO_CLIENT_NOT_JOINED);
    
    Packet packet = Leaving();
    send(packet);
    joined_ = false;
  }
}
