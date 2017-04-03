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
  const size_t bufferSize = 255;
  uint8_t buffer[bufferSize];
  Client::Client(string name, unsigned serverPort, string serverIP,
                 double timeout)
  : Node(name)
  {
    // Initialize server address
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.data());
    serverAddr.sin_port = htons(serverPort);
    
    // Send a join request
    JoinRequest pkt(name);
    send(pkt);
    
    // Wait for, then process, the server's response
    clock_t initial_clock = clock();
    while((clock() - initial_clock)/CLOCKS_PER_SEC <= timeout)
    {
      struct sockaddr_in recvAddr;
      socklen_t tmpLen = sizeof(recvAddr);
      ssize_t length = recvfrom(sock, buffer, bufferSize, 0,
                                (struct sockaddr*) &recvAddr, &tmpLen);
      // Process anything recieved
      if(length > 0)
      {
        // Populate a mystery packet
        MysteryPacket pkt;
        pkt.populate((buffer));
        if(pkt.isType<JoinResponse>())
        {
          // Process the recieved join response
          JoinResponse joinResponse(pkt);
          if(joinResponse.ok())
          {
            // Join successful, set everything up
            joined = true;
            ID = joinResponse.assignedID();
            maxID = joinResponse.maxID();
            used = vector<bool>(maxID, false);
            used[0] = true;
            used[ID] = true;
            names.resize(getMaxNodes());
            names[0] = joinResponse.serverName();
            names[ID] = name;
            serverAddr = recvAddr;
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
    if(joined)
      send(Leaving());
    close(sock);
  }
  void Client::send(const AbstractPacket& packet) const
  {
    size_t size = AbstractPacket::HEADER_SIZE + packet.getSize();
    packet.toBuffer(buffer, ID);
    sendto(sock, buffer, size, 0, (struct sockaddr*) &serverAddr, lenAddr);
  }
  bool Client::recv(MysteryPacket& result)
  {
    // Pull data from the socket
    struct sockaddr_in recvAddr;
    socklen_t tmpLen = sizeof(recvAddr);
    ssize_t length = recvfrom(sock, buffer, bufferSize, 0,
                              (struct sockaddr*) &recvAddr, &tmpLen);
    if(length > 0)
    {
      // Verify data comes from server and populate a mystery packet
      if(recvAddr.sin_addr.s_addr == serverAddr.sin_addr.s_addr &&
         recvAddr.sin_port == serverAddr.sin_port)
      {
        result.populate(buffer);
        
        // Characterize and process the mystery packet
        if(result.isType<ClientJoined>())
        {
          ClientJoined clientJoined(result);
          used[clientJoined.newID()] = true;
          names[clientJoined.newID()] = clientJoined.newName();
        }
        else if(result.isType<ClientInfo>())
        {
          ClientInfo clientInfo(result);
          used[clientInfo.ID()] = true;
          names[clientInfo.ID()] = clientInfo.name();
        }
        else if(result.isType<Kick>() ||
                result.isType<Ban>()  ||
                result.isType<Shutdown>())
        {
          joined = false;
        }
        else if(result.isType<ClientLeft>())
        {
          ClientLeft clientLeft(result);
          used[clientLeft.oldID()] = false;
        }
        return true;
      }
      throw Error("packet recieved from unknown source");
    }
    return false;
  }
}
