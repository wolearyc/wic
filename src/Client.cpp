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
 * File:    Client.cpp
 * ----------------------------------------------------------------------------
 */
#include "Client.h"
namespace wic
{
  const size_t BUFFER_SIZE_ = 255;
  uint8_t buffer_[255];

  Client::Client(string name, unsigned serverPort, string serverIP,
                 double timeout)
  : Node(name, 0)
  {
    // Initialize server address
    bzero(&serverAddr_, sizeof(serverAddr_));
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = inet_addr(serverIP.data());
    serverAddr_.sin_port = htons(serverPort);
    
    // Send a join request
    JoinRequest pkt(name_);
    send(pkt);
    
    // Wait for, then process, the server's response
    clock_t initial_clock = clock();
    while((clock() - initial_clock)/CLOCKS_PER_SEC <= timeout)
    {
      struct sockaddr_in recvAddr;
      socklen_t tmpLen = sizeof(recvAddr);
      ssize_t length = recvfrom(socket_, buffer_, BUFFER_SIZE_, 0,
                                (struct sockaddr*) &recvAddr, &tmpLen);
      // Process anything recieved
      if(length > 0)
      {
        // Populate a mystery packet
        MysteryPacket pkt;
        pkt.populate((buffer_));
        if(pkt.isType<JoinResponse>())
        {
          // Process the recieved join response
          JoinResponse joinResponse(pkt);
          if(joinResponse.ok())
          {
            // Join successful, set everything up
            joined_ = true;
            ID_ = joinResponse.assignedID();
            maxNodes_ = joinResponse.maxNodes();
            used_ = vector<bool>(maxNodes_, false);
            used_[0] = true;
            used_[ID_] = true;
            names_.resize(maxNodes_);
            names_[0] = joinResponse.serverName();
            names_[ID_] = name_;
            serverAddr_ = recvAddr;
            return;
          }
          else if(joinResponse.full())
            throw Failure("server full");
          else if(joinResponse.banned())
            throw Failure("banned");
        }
      }
    }
    throw Failure("timeout");
  }
  Client::~Client() 
  {
    Leaving pkt = Leaving();
    send(pkt);
    close(socket_);
  }
  void Client::send(const AbstractPacket& packet) const
  {
    size_t size = AbstractPacket::HEADER_SIZE + packet.size();
    packet.toBuffer(buffer_, ID_);
    sendto(socket_, buffer_, size, 0, (struct sockaddr*) &serverAddr_, lenAddr_);
  }
  bool Client::recv(MysteryPacket& result)
  {
    // Pull data from the socket
    struct sockaddr_in recvAddr;
    socklen_t tmpLen = sizeof(recvAddr);
    ssize_t length = recvfrom(socket_, buffer_, BUFFER_SIZE_, 0,
                              (struct sockaddr*) &recvAddr, &tmpLen);
    if(length > 0)
    {
      // Verify data comes from server and populate a mystery packet
      if(recvAddr.sin_addr.s_addr == serverAddr_.sin_addr.s_addr &&
         recvAddr.sin_port == serverAddr_.sin_port)
      {
        result.populate(buffer_);
        
        // Characterize and process the mystery packet
        if(result.isType<ClientJoined>())
        {
          ClientJoined clientJoined(result);
          used_[clientJoined.newID()] = true;
          names_[clientJoined.newID()] = clientJoined.newName();
        }
        else if(result.isType<ClientInfo>())
        {
          ClientInfo clientInfo(result);
          used_[clientInfo.ID()] = true;
          names_[clientInfo.ID()] = clientInfo.name();
        }
        else if(result.isType<Kick>() ||
                result.isType<Ban>()  ||
                result.isType<Shutdown>())
        {
          joined_ = false;
        }
        else if(result.isType<ClientLeft>())
        {
          ClientLeft clientLeft(result);
          used_[clientLeft.oldID()] = false;
        }
        return true;
      }
      throw Error("packet recieved from unknown source");
    }
    return false;
  }
}
