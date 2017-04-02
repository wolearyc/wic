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
  /** A server node that connects to multiple client nodes. */
  class Server : public Node
  {
  public:
    /** Constructor (starts server).
     *  \param name server's name; limited to 20 characters
     *  \param port port number on which to listen for packets; must be >1024
     *  \param maxClients maximum simultaneously connected clients; must be
     *         in the range 1-254
     */
    Server(string name, unsigned port, uint8_t maxClients);
    ~Server();
    /** Sends a packet to a single client.
     *  \param packet the packet to send
     *  \param destID the ID of the recipient
     */
    void send(const AbstractPacket& packet, NodeID destID) const;
    /** Sends a packet to all clients except one.
     *  \param packet the packet to send
     *  \param excludeID the ID of the excluded client
     */
    void sendExclude(const AbstractPacket& packet, NodeID excludeID) const;
    /** Broadcasts a packet to all clients
     *  \param packet the packet to send
     */
    void sendAll(const AbstractPacket& packet) const;
    /** Attempts to recieve a single packet. 
     *  \param result the destination of the received packet
     *  \return true if packet recieved, false otherwise
     */
    bool recv(MysteryPacket& result);
    /** Kicks a client by ID.
     *  \param ID the ID of the client to be kicked
     *  \param reason the reason; limited to 50 characters
     */
    void kick(NodeID ID, string reason);
    /** Kicks a client by name or IP.
     *  \param nameOrIP the client's name or IP address
     *  \param reason the reason; limited to 50 characters
     */
    void kick(string nameOrIP, string reason);
    /** Kicks a client by ID and prevents that client from reconnecting.
     *  \param ID the ID of the client to be banned
     */
    void ban(NodeID ID);
    /** Bans a name or IP address.
     *  \param nameOrIP a name or IP address
     */
    void ban(string nameOrIP);
    /** Unbans a name or IP address.
     *  \param nameOrIP a name or IP address
     */
    void unban(string nameOrIP);
    /** Returns the ID of the client with a name or IP address */
    NodeID getNodeID(string nameOrIP) const;
  private:
    vector<string> ips;
    vector<string> blacklist;
    vector<struct sockaddr_in> addrs;
  };
}
#endif
