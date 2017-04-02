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
 * File:    Node.cpp
 * ----------------------------------------------------------------------------
 */
#include "Node.h"
namespace wic
{
  Node::Node(string name, unsigned socketPort)
  : joined(false), ID(0), name(name), maxID(0), sock(0),
    lenAddr(sizeof(sockaddr_in))
  {
    if(name.length() > MAX_NAME_LEN)
      throw InvalidArgument("name", "> " + std::to_string(MAX_NAME_LEN));
    if(socketPort < 1025)
      throw InvalidArgument("port", "< 1025");
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
      throw InternalError("socket could not initialize");
    fcntl(sock, F_SETFL, O_NONBLOCK);
    bzero(&addr, lenAddr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(socketPort);
    int result = bind(sock, (struct sockaddr*) &addr, lenAddr);
    if(result == -1)
    {
      close(sock);
      if(errno == EADDRINUSE)
        throw Failure("port already in use");
      else
        throw InternalError("socket could not bind");
    }
  }
  NodeID Node::getID() const
  {
    return ID;
  }
  string Node::getName() const
  {
    return name;
  }
  NodeID Node::getMaxID() const
  {
    return maxID;
  }
  uint8_t Node::getMaxNodes() const
  {
    return 1 + maxID;
  }
  bool Node::isUsed(NodeID ID) const
  {
    if(ID > maxID)
      return false;
    return used[ID];
  }
  string Node::getNodeName(NodeID ID) const
  {
    return names[ID];
  }
  const uint8_t Node::MAX_NAME_LEN = 20;

}
