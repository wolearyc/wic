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
 * File:    Server.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef SERVER_H
#define SERVER_H
#include "Packet.h"
namespace wic
{
  /** \brief a simple UDP server that connects to multiple clients
   *
   *  A Server sends and recieves packets to and from multiple clients.
   *  Server handles certain low level functions, such as joining, kicking,
   *  and banning clients (by name or IP). More advanced features can be 
   *  implemented by users by pulling recieved packets out of a Server and 
   *  processing them accordingly.
   */
  class Server : public Node
  {
  public:
    /** \brief Constructor (starts server)
     *  \param name server's name; limited to 20 characters
     *  \param port port number on which to listen for packets; must be >1024
     *  \param maxClients maximum simultaneously connected clients; must be
     *         in the range 1-254
     */
    Server(string name, unsigned port, uint8_t maxClients);
    ~Server();
    /** \brief sends a packet to a single client
     *  \param packet the packet to send
     *  \param destID the ID of the recipient
     */
    void send(const AbstractPacket& packet, NodeID destID) const;
    /** \brief sends a packet to all but one clients
     *  \param packet the packet to send
     *  \param excludeID the ID of the excluded client
     */
    void sendExclude(const AbstractPacket& packet, NodeID excludeID) const;
    /** \brief broadcasts a packet to all clients
     *  \param packet the packet to send
     */
    void sendAll(const AbstractPacket& packet) const;
    /** \brief fetches and processes a single recieved packet
     *  \param result the destination of the received packet
     *  \return true if packet recieved, false otherwise
     */
    bool recv(MysteryPacket& result);
    /** \brief kicks a client
     *  \param ID the ID of the client to be kicked
     *  \param reason the reason; limited to 50 characters
     */
    void kick(NodeID ID, string reason);
    /** \brief kicks a client
     *  \param nameOrIP the client's name or IP address
     *  \param reason the reason; limited to 50 characters
     */
    void kick(string nameOrIP, string reason);
    /** \brief kicks a client and prevents them from reconnecting
     *  \param ID the ID of the client to be banned
     */
    void ban(NodeID ID);
    /** \brief bans a name or IP address
     *  \param nameOrIP a name or IP address
     */
    void ban(string nameOrIP);
    /** \brief unbans a name or IP address
     *  \param nameOrIP a name or IP address
     */
    void unban(string nameOrIP);
    /** \brief returns the ID of the client with a name or IP address */
    NodeID getID(string nameOrIP) const;
  private:
    vector<string> ips_;
    vector<string> blacklist_;
    vector<struct sockaddr_in> addrs_;
  };
}
#endif
