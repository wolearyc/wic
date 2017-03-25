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
 * File:    Client.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef CLIENT_H
#define CLIENT_H
#include "Packet.h"
#include "Error.h"
using std::string;
using std::vector;
namespace wic
{
  /** \brief a simple UDP client that connects to a server
   *
   *  A Client works by sending and recieving packets to and from a server.
   *  Client handles certain low level functions, such as joining and leaving.
   *  More advanced features can be implemented by users by pulling received
   *  packets out of a Client and processing them accordingly. Only
   *  one Client can be initialized at a time in a game.
   *  Since Client uses UDP, packets are likely, but not guaranteed, to arrive
   *  in order. Packets may not even arrive at all.
   */
  class Client
  {
  public:
    /** \brief Constructor
     *  \param name the client's name, up to 20 characters
     */
    Client(string name);
    /** \brief Default constructor */
    Client();
    ~Client();
    /** \brief attempts to join a client to its server
     *  \param port the server port; must
     *  \param ip the server IP address
     *  \param timeout the join timeout in seconds
     *  \return the server's response packet
     */
    Packet join(unsigned port, string ip, double timeout);
    /** \brief sends a packet to a client's server
     *  \param packet the packet to send
     */
    void send(Packet& packet);
    /** \brief attempted to fetch and process a single packet from the server
     *  \param result the destination of the received packet
     */
    void recv(Packet& result);
    /** \brief leaves the server
     */
    void leave();
  private:
    bool joined_;                    /**< whether or not  joined */
    NodeID ID_;                      /**< server-assigned ID */
    string name_;                    /**< name */
    uint8_t maxNodes_;               /**< max nodes supported by server */
    vector<bool> used_;              /**< ID-indexed used list */
    vector<string> names_;           /**< ID-indexed names */
    int socket_;                     /**< socket */
    socklen_t lenAddr_;              /**< address length */
    struct sockaddr_in addr_;        /**< address */
    struct sockaddr_in serverAddr_;  /**< server address */
  };
}
#endif
