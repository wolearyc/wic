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
  /** A node ID. */
  typedef uint8_t NodeID;
  /** A UDP node. Each node possesses a name and a unique integer ID. */
  class Node
  {
  public:
    /** Constructor (binds socket to port).
     *  \param name name of the node; limited to 20 characters
     *  \param socketPort the port on which to bind the socket; must be > 1025
     *  \exception Failure "port already in use"
     */
    Node(string name, unsigned socketPort);
    /** Constructor (binds socket to any port).
     *  \param name name of the node; limited to 20 characters
     */
    Node(string name);
    /** Returns the unique ID. */
    NodeID getID() const;
    /** Returns the name. */
    string getName() const;
    /** Returns the maximum allowed ID. */
    NodeID getMaxID() const;
    /** Returns the maximum number of connected nodes. */
    uint8_t getMaxNodes() const;
    /** Returns whether or not an ID is in use */
    bool isUsed(NodeID ID) const;
    /** Returns the name currently or previously associated with an ID. */
    string getNodeName(NodeID ID) const;
  protected:
    void bindSocket(unsigned socketPort);
    static const uint8_t MAX_NAME_LEN;
    bool joined;
    NodeID ID;
    string name;
    NodeID maxID;
    vector<bool> used;
    vector<string> names;
    int sock;
    socklen_t lenAddr;
    struct sockaddr_in addr;
  };
}
#endif
