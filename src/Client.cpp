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
  static const size_t bufferSize = 256; // utility buffer size
  static uint8_t buffer[bufferSize];    // utility buffer
  
  // Utility packets
  static JoinResponse joinResponse;
  static ClientJoined clientJoined;
  static ClientInfo clientInfo;
  static Kick kick;
  static Ban ban;
  static Shutdown shutdown;
  static ClientLeft clientLeft;
  
  Client::Client(string name, unsigned serverPort, string serverIP,
                 double timeout)
  : joined_(false), ID_(0), name_(""), maxNodes_(0), socket_(0),
    lenAddr_(sizeof(sockaddr_in))
  {
    if(name.length() > 20)
      throw InvalidArgument("name", "limited to 20 characters");
    name_ = name;
    
    if(serverPort < 1025)
      throw InvalidArgument("port", "only ports above 1024 are allowed");
    
    bzero(&addr_, sizeof(addr_));
    bzero(&serverAddr_, sizeof(serverAddr_));

    // Bind socket
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_ == -1)
      throw InternalError("socket initialization failed");
    fcntl(socket_, F_SETFL, O_NONBLOCK);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(0);
    int result = bind(socket_, (struct sockaddr*) &addr_, sizeof(addr_));
    if(result == -1)
    {
      close(socket_);
      if(errno == EADDRINUSE)
        throw Failure("port is already in use");
      else
        throw InternalError("socket bind failed");
    }
    
    // Initialize server address
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = inet_addr(serverIP.data());
    serverAddr_.sin_port = htons(serverPort);
    
    // Send a join request
    JoinRequest pkt;
    pkt.populate(name_);
    send(pkt);
    
    // Wait for, then process, the server's response
    clock_t initial_clock = clock();
    while((clock() - initial_clock)/CLOCKS_PER_SEC <= timeout)
    {
      struct sockaddr_in recvAddr;
      socklen_t tmpLen = sizeof(recvAddr);
      ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                                (struct sockaddr*) &recvAddr, &tmpLen);
      // Process anything recieved
      if(length > 0)
      {
        // Populate a mystery packet
        MysteryPacket pkt;
        pkt.populate(buffer);
        
        if(pkt.isType(joinResponse))
        {
          // Process the recieved join response
          joinResponse.populate(pkt);
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
    joined_ = false;
    close(socket_);
  }
  void Client::send(const Packet& packet) const
  {
    size_t size = Packet::HEADER_SIZE + packet.size();
    packet.toBuffer(buffer, ID_);
    sendto(socket_, buffer, size, 0, (struct sockaddr*) &serverAddr_, lenAddr_);
  }
  bool Client::recv(MysteryPacket& result)
  {
    // Pull data from the socket
    struct sockaddr_in recvAddr;
    socklen_t tmpLen = sizeof(recvAddr);
    ssize_t length = recvfrom(socket_, buffer, bufferSize, 0,
                              (struct sockaddr*) &recvAddr, &tmpLen);
    if(length > 0)
    {
      // Verify data comes from server and populate a mystery packet
      if(recvAddr.sin_addr.s_addr == serverAddr_.sin_addr.s_addr &&
         recvAddr.sin_port == serverAddr_.sin_port)
      {
        result.populate(buffer);
        
        // Characterize and process the mystery packet
        if(result.isType(clientJoined))
        {
          clientJoined.populate(result);
          used_[clientJoined.newID()] = true;
          names_[clientJoined.newID()] = clientJoined.newName();
        }
        else if(result.isType(clientInfo))
        {
          clientInfo.populate(result);
          used_[clientInfo.ID()] = true;
          names_[clientInfo.ID()] = clientInfo.name();
        }
        else if(result.isType(kick) ||
                result.isType(ban)  ||
                result.isType(shutdown))
        {
          joined_ = false;
        }
        else if(result.isType(clientLeft))
        {
          clientLeft.populate(result);
          used_[clientLeft.oldID()] = false;
        }
        return true;
      }
      throw Error("packet recieved from unknown source");
    }
    return false;
  }
  NodeID Client::ID() const
  {
    return ID_;
  }
  string Client::name() const
  {
    return name_;
  }
  NodeID Client::getMaxID() const
  {
    return maxNodes_;
  }
  bool Client::isUsed(NodeID ID)
  {
    if(ID > getMaxID())
      return false;
    return used_[ID];
  }
  string Client::getName(NodeID ID)
  {
    if(!isUsed(ID))
      throw InvalidArgument("ID", "ID is not associated with any client");
    return names_[ID];
  }
}
