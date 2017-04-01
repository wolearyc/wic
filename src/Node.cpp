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
  const uint8_t Node::NAME_LEN = 20;
  Node::Node(string name, unsigned socketPort)
  : joined_(false), ID_(0), name_(name), maxNodes_(0), socket_(0),
    lenAddr_(sizeof(sockaddr_in))
  {
    if(name_.length() > NAME_LEN)
      throw InvalidArgument("name", "> " + std::to_string(NAME_LEN));
    if(socketPort < 1025)
      throw InvalidArgument("port", "< 1025");
    
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_ == -1)
      throw InternalError("socket initialization error");
    fcntl(socket_, F_SETFL, O_NONBLOCK);
    bzero(&addr_, lenAddr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_.sin_port = htons(socketPort);
    int result = bind(socket_, (struct sockaddr*) &addr_, lenAddr_);
    if(result == -1)
    {
      close(socket_);
      if(errno == EADDRINUSE)
        throw Failure("port already in use");
      else
        throw InternalError("socket binding error");
    }
  }
  NodeID Node::ID() const
  {
    return ID_;
  }
  string Node::name() const
  {
    return name_;
  }
  NodeID Node::getMaxID() const
  {
    return maxNodes_;
  }
  bool Node::isUsed(NodeID ID) const
  {
    if(ID > getMaxID())
      return false;
    return used_[ID];
  }
  string Node::getName(NodeID ID) const
  {
    if(!isUsed(ID))
      throw InvalidArgument("ID", "unused");
    return names_[ID];
  }
}
