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
 * File:    Packet.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef PACKET_H
#define PACKET_H
#include <stdlib.h>
#include <time.h>
#include <string.h>
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
#include <vector>
#include "Error.h"
using std::string;
using std::vector;
namespace wic
{
  /** \brief the maximum number of bytes alloted to any wic server or client name
   */
  extern const uint8_t NAME_SIZE;
  /** \brief identifies the index of a node (either the server or a client) */
  typedef uint8_t NodeID;
  /** \brief the server's index (always 0) */
  extern const NodeID SERVER_ID;
  
  /** \brief a packet containing source and type information as well as
   *         a data payload of a certain size
   */
  class Packet
  {
  friend class Client;
  friend class Server;
  public:
    /** \brief Initializes a packet from a buffer.
     *
     *  \param buffer a buffer
     */
    Packet(uint8_t* buffer);
    Packet();
    Packet(const Packet& packet);
    void fromBuffer(uint8_t* src);
    /** \brief Converts packet into a buffer.
     *
     *  \param dest destination buffer
     */
    void toBuffer(uint8_t* dest);
    /** \brief returns the packet's ID
     *
     *  This function should be implemented in subclasses of Packet. id should be
     *  a constant >16, as Wic reserves lower IDs.
     */
    uint8_t type();
    /** \brief returns the packet's payload size
     *
     *  This function must be implemented in subclasses of Packet.
     */
    uint8_t size();
    static const size_t HEADER_SIZE;
  protected:
    NodeID source_;           /**< the node id of the sender */
    vector<uint8_t> data_;    /**< the data payload */
  private:
    uint8_t type_;            /**< packet type ID */
    uint8_t size_;
  };
  
  /** \brief the reserved packet a client sends the server when attempting to 
   *         join
   *
   *  This packet contains the 21 byte name of the client.
   */
  class JoinRequest : public Packet
  {
  public:
    static const uint8_t type = 0;
    static const uint8_t size = 21;
    JoinRequest(string name);
    JoinRequest(const Packet& packet);
    string name();
  };
  
  /** \brief the reserved packet a server sends to a client in response to 
   *         a join request
   *
   *  This packet contains 24 bytes of data: first, a join response code.
   *  Second, the number of nodes the server supports (1 + maxiumum number of
   *  clients). Third, the newly connected client's assigned index.
   *  Fourth, the 21 byte name of the server.
   */
  class JoinResponse : public Packet
  {
  public:
    static const uint8_t type = 1;
    static const uint8_t size = 24;
    JoinResponse(uint8_t responseCode, uint8_t numNodes, NodeID assignedID,
                 string serverName);
    JoinResponse(const Packet& packet);
    bool joined();
    bool full();
    bool banned();
    uint8_t numNodes();
    NodeID assignedID();
    string serverName();
  public:
    static const uint8_t OK;     /**< successful join  code */
    static const uint8_t FULL;   /**< failed join code due to full server */
    static const uint8_t BANNED; /**< failed join code due to client ban */
  };
  /** \brief the reserved packet recieved by server and all previously connected
   *         clients announcing that a new client has successfully joined
   *
   *  This packet contains 22 bytes of data. First, the index of the newly joined
   *  client. Second, the 21 byte name of the newly joined client.
   */
  class ClientJoined : public Packet
  {
  public:
    static const uint8_t type = 2;
    static const uint8_t size = 22;
    ClientJoined(NodeID newID, string newName);
    ClientJoined(const Packet& packet);
    NodeID newID();
    string newName();
  };
  /** \brief the reserved packet sent from a server to a client that just joined
   *         announcing a single client already in the server
   *  This packet contains 22 bytes of data. First, the index of the client
   *  already in the server. Second, the 21 byte name of the client already in
   *  the server.
   */
  class ClientInfo : public Packet
  {
  public:
    static const uint8_t type = 3;
    static const uint8_t size = 22;
    ClientInfo(NodeID ID, string name);
    ClientInfo(const Packet& packet);
    NodeID ID();
    string name();
  };
  /** \brief the reserved packet sent from a client to a server indicating the
   *         client has left
   *
   *  This packet contains no data.
   */
  class Leaving : public Packet
  {
  public:
    static const uint8_t type = 4;
    static const uint8_t size = 0;
    Leaving();
    Leaving(const Packet& packet);
  };
  /** \brief the reserved packet sent from a server to a client indicating that
   *         the client has been kicked from the server
   *
   *  This packet contains a 51 byte string explaining the reason for the kick.
   */
  class Kick : public Packet
  {
  public:
    static const uint8_t type = 5;
    static const uint8_t size = 51;
    Kick(string reason);
    Kick(const Packet& packet);
    string reason();
  };
  /** \brief the reserved packet sent from a server to a client indicating that
   *         the client has been banned from the server
   *
   *  This packet contains a 51 byte string explaining the reason for the ban.
   */
  class Ban : public Packet
  {
  public:
    static const uint8_t type = 6;
    static const uint8_t size = 51;
    Ban(string reason);
    Ban(const Packet& packet);
    string reason();
  };
  /** \brief the reserved packet sent from a server to >1 clients indicating that
   *         another client has left
   *
   *  This packet contains 53 bytes of data. First, the index of the client who
   *  left. Second, the 1 byte leave code. Third, a 51 byte string explaining why
   *  the client left. In the case of kick or ban, this string will be the
   *  explaination of the kick or ban. Else, the string will be empty.
   */
  class ClientLeft : public Packet
  {
  public:
    static const uint8_t type = 7;
    static const uint8_t size = 53;
    ClientLeft(NodeID oldID, uint8_t leaveCode, string reason);
    ClientLeft(const Packet& packet);
    NodeID oldID();
    bool normal();
    bool kicked();
    bool banned();
    string reason();
    static const uint8_t NORMAL;     /**< left by choice code */
    static const uint8_t KICKED; /**< kicked code */
    static const uint8_t BANNED; /**< banned code */
  };
  /** \brief the reserved packet sent from a server to all joined clients
   *         indicating server shutdown
   *
   *  This packet contains no data.
   */
  class Shutdown : public Packet
  {
  public:
    static const uint8_t type = 8;
    static const uint8_t size = 0;
    Shutdown();
    Shutdown(const Packet& packet);
  };
  bool wic_is_reserved_packet_id(uint8_t packet_id);
}
#endif
