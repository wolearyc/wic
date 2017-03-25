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
 * File:    Server.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef SERVER_H
#define SERVER_H
#include "Error.h"
#include "Packet.h"
using std::string;
using std::vector;
namespace wic
{
  /** \brief a simple UDP server that connects to multiple clients
   *
   *  A Server works by sending and recieving packets to and from players.
   *  Server handles certain low level functions, such as joining,  kicking,
   *  and banning players. More advanced features can be implemented by users by
   *  pulling recieved packets out of a Server and processing them accordingly.
   *  Only one Server can be initialized at a time.
   *
   *  Since Server uses UDP, packets are likely, but not guaranteed, to arrive
   *  in order. Packets may not even arrive at all.
   */
  class Server
  {
  public:
    /** \brief initializes a Server, allowing remote clients to connect
     *  \param name the desired name of the server; must have 1-20 characters
     *  \param port the desired port
     *  \param maxClients the desired maximum number of connected clients; must be
     *         in the range 1-254
     */
    Server(string name, unsigned port, uint8_t maxClients);
    ~Server();
    /** \brief sends a single packet to a client
     *  \param packet the packet to send
     *  \param destID the index of the client to sent it to; must be > 0
     */
    void send(Packet& packet, NodeID destID);
    /** \brief sends a packet to all connected clients but one
     *  \param packet the packet to send
     *  \param excludeID the index of the client to exclude; must be > 0
     */
    void sendExclude(Packet& packet, NodeID excludeID);
    /** \brief sends a packet to all connected clients
     *  \param packet the packet to send
     */
    void sendAll(Packet& packet);
    /** \brief fetches and processes a single packet from a client
     *  \param result the destination of the received packet
     */
    void recv(Packet& result);
    /** \brief kicks a client
     *  \param ID the client's index; must be > 0
     *  \param reason string given the reason for kick; must be 0-50 characters
     */
    void kick(NodeID ID, string reason);
    /** \brief kicks a client
     *  \param nameOrIP the client's name or IP address
     *  \param reason string given the reason for kick; must be 0-50 characters
     */
    void kick(string nameOrIP, string reason);
    /** \brief bans a client by ID
     *  \param ID the client's index; must be > 0
     */
    void ban(NodeID ID);
    /** \brief bans a name or IP address
     *  \param nameOrIP a name or IP address
     */
    void ban(string nameOrIP);
    /** \brief unbans a client name of IP address
     *  \param nameOrIP a name or IP address
     */
    void unban(string nameOrIP);
    /** \brief returns the node index of a client with a particular name/IP
     *  \return index on success, 0 on failure
     */
    NodeID getID(string nameOrIP);
  private:
    string name_;
    uint8_t maxNodes_;
    vector<bool> used_;
    vector<string> names_;
    vector<string> ips_;
    vector<string> blacklist_;
    int socket_;
    socklen_t lenAddr_;
    struct sockaddr_in addr_;
    vector<struct sockaddr_in> addrs_;
  };
}
#endif
