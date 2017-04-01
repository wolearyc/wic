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
 * File:    Client.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef CLIENT_H
#define CLIENT_H
#include "Node.h"
#include "Packet.h"
namespace wic
{
  /** \brief a UDP client that connects to a server
   *
   *  A Client sends and recieves packets to and from the server. It handles
   *  certain low-level functions, such as joining and leaving.
   *  More advanced features should be implemented by pulling recieved packets
   *  out of a client and processing them accordingly. 
   *
   *  Each client has an unique nonzero ID assigned by the server. Every client
   *  also possesses a name (username), which need not be unique and can
   *  can certainly be empty.
   */
  class Client : public Node
  {
  public:
    /** \brief Constructor (joins server)
     *  \param name the username; limited to 20 characters
     *  \param serverPort the server port number; must be > 1024
     *  \param serverIP the server IP address
     *  \param timeout the time, in seconds, to wait for server response
     */
    Client(string name, unsigned serverPort, string serverIP, double timeout);
    ~Client();
    /** \brief sends a packet to the server
     *  \param packet the packet to send
     */
    void send(const AbstractPacket& packet) const;
    /** \brief attempts to fetch and process a single packet from the server
     *  \param result the destination of recieved packet
     *  \return true if packet recieved, false otherwise
     */
    bool recv(MysteryPacket& result);
  private:
    struct sockaddr_in serverAddr_;
  };
}
#endif
