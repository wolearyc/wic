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
 * File:    Node.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef NODE_H
#define NODE_H
#include <vector>
#include <time.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "Error.h"
using std::string;
using std::vector;
namespace wic
{
  /** \brief a node ID */
  typedef uint8_t NodeID;
  /** \brief a UDP node that sends and recieves packets
   *
   *  A Client sends and recieves packets to and from the server. It handles
   *  certain low-level functions, such as joining and leaving.
   *  More advanced features should be implemented by pulling recieved packets
   *  out of a client and processing them accordingly. 
   *
   *  Each node has an unique nonzero ID. Every node also possesses a name
   *  (username), which need not be unique and can certainly be empty.
   */
  class Node
  {
  public:
    static const uint8_t NAME_LEN; /**< defines maximum name length */
    /** \brief Constructor (binds socket)
     *  \param socketPort the port on which to bind the socket
     */
    Node(string name, unsigned socketPort);
    /** \brief returns the unique ID */
    NodeID ID() const;
    /** \brief returns the name */
    string name() const;
    /** \brief gets the maximum allowed ID */
    NodeID getMaxID() const;
    /** \brief returns whether or not an ID is in use */
    bool isUsed(NodeID ID) const;
    /** \brief returns the name associated with a certain ID */
    string getName(NodeID ID) const;
  protected:
    bool joined_;
    NodeID ID_;
    string name_;                    
    uint8_t maxNodes_;
    vector<bool> used_;
    vector<string> names_;
    int socket_;                     
    socklen_t lenAddr_;              
    struct sockaddr_in addr_;
    struct sockaddr_in recvAddr_;
    socklen_t lenRecvAddr_;
  };
}
#endif
