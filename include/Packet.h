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
  /** \brief the maximum length of names */
  extern const uint8_t NAME_SIZE;
  /** \brief a node ID */
  typedef uint8_t NodeID;
  /** \brief the server's ID (always 0) */
  extern const NodeID SERVER_ID;
  /** \brief Data exchanged between a server and clients
   *  
   *  A packet has a source (sender ID), a type (what sort of data is it),
   *  a size (how much data is there), and finally a data payload. Due to 
   *  the large amounts of data that must be processed, users should
   *  initialize objects for all types of packets. Recieved "mystery packets"
   *  can be used to populate the data of specific packets. Specific packets can
   *  be populated with user-defined populate methods and then sent over the
   *  network.
   */
  class Packet
  {
    friend class Client;
    friend class Server;
  public:
    /** \brief Populate packet based on another
     *  This method commonly used to convert a mystery packet into a more
     *  workable form. The types of the two packets must agree.
     *  \param other another packet
     */
    void populate(const Packet& other);
    /** \brief Returns the data payload */
    vector<uint8_t> data() const;
    /** \brief Returns the ID of the sender */
    NodeID source() const;
    /** \brief Determines whether or not another packet is of the same type */
    bool isType(const Packet& other) const;
    /** \brief the type
     *  This method defines the type. When implementing specific packets, this
     *  function should return a unique constant.
     */
    virtual uint8_t type() const = 0;
    /** \brief the size
     *  This method defines the size. When implemting specific packets, this
     *  function should return a constant.
     */
    virtual uint8_t size() const = 0;
  protected:
    vector<uint8_t> data_;
    NodeID source_;
    static const size_t HEADER_SIZE;
  private:
    void toBuffer(uint8_t* dest, NodeID source) const;
  };
  
  /** \brief Packet of undetermined type, typically recieved */
  class MysteryPacket : public Packet
  {
    friend class Client;
    friend class Server;
  public:
    uint8_t type() const;
    uint8_t size() const;
  private:
    void populate(uint8_t* src);
    uint8_t type_;
    uint8_t size_;
  };
  
  /** \brief packet a client sendsto a server when requesting to join
   *  This packet contains the 21 byte name of the client.
   */
  class JoinRequest : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 0; }
    uint8_t size() const { return 21; }
    void populate(string name);
    string name();
  };
  
  /** \brief the packet a server sends to a client to respond to a join request
   *
   *  This packet contains 24 bytes of data. The first byte is a join response
   *  code, which indicates whether joining is allowed or disallowed due to 
   *  a full server or a ban on the clients name or IP. If the client is allowed
   *  to join, the remainder of the data is set. The second byte is the server's
   *  maximum number of nodes. The third byte is the new ID assigned to the
   *  client. The remainder of the data is the name of the server.
   */
  class JoinResponse : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 1; }
    uint8_t size() const { return 24; }
    void populate(uint8_t responseCode, uint8_t numNodes, NodeID assignedID,
                  string serverName);
    /** \brief returns whether or not join is ok */
    bool ok() const;
    /** \brief returns whether or not join failed due to a full server */
    bool full() const;
    /** \brief returns whether or not join failed because the client is banned */
    bool banned() const;
    /** \brief returns the maximum number of nodes the server supports */
    uint8_t maxNodes() const;
    /** \brief returns the new ID assigned to the client */
    NodeID assignedID() const;
    /** \brief returns the server's name */
    string serverName() const;
    static const uint8_t OK;     /**< successful join  code */
    static const uint8_t FULL;   /**< failed join code due to full server */
    static const uint8_t BANNED; /**< failed join code due to client ban */
  };
  /** \brief the packet recieved by all nodes announcing that a new client has 
   *         joined
   *  This packet contains 22 bytes of data. The first byte is the assigned ID
   *  of the new client. The remaining data is the name of the new client.
   */
  class ClientJoined : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 2; }
    uint8_t size() const { return 22; }
    void populate(NodeID newID, string newName);
    /** \brief returns the new client's ID */
    NodeID newID() const;
    /** \brief returns the new client's name */
    string newName() const;
  };
  /** \brief the packet sent from a server to recently joined client notifying
   *         the 'new' client of other existing clients
   *  This packet contains 22 bytes of data. The first byte is the ID
   *  of the existing client. The remaining data is the name of the existing client.
   */
  class ClientInfo : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 3; }
    uint8_t size() const { return 22; }
    void populate(NodeID ID, string name);
    /** \brief returns the existing client's ID */
    NodeID ID() const;
    /** \brief returns the existing client's name */
    string name() const;
  };
  /** \brief the packet sent from a client to a server when the client leaves */
  class Leaving : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 4; }
    uint8_t size() const { return 0; }
  };
  /** \brief the reserved packet sent from a server to a client indicating that
   *         the client has been kicked
   *
   *  This packet contains a 51 bytes, a string explaining the reason for the 
   *  kick.
   */
  class Kick : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 5; }
    uint8_t size() const { return 51; }
    void populate(string reason);
    /** \brief returns the reason for the kick */
    string reason() const;
  };
  /** \brief the packet sent from a server to a client indicating that
   *         the client has been banned
   *
   *  This packet contains a 51 byte string explaining the reason for the ban.
   */
  class Ban : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 6; }
    uint8_t size() const { return 51; }
    void populate(string reason);
    /** \brief returns the reason for the ban */
    string reason() const;
  };
  /** \brief the packet sent from a server to all connected clients indicating
   *         that a client has left.
   *
   *  This packet contains 53 bytes of data. The first byte is the ID of the
   *  client that left. The second byte is the leave code, indicating whether
   *  the client left normally, was kicked, or was banned. The remaining data
   *  is the reason for the kick/ban, if applicable. 
   */
  class ClientLeft : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 7; }
    uint8_t size() const { return 53; }
    void populate(NodeID oldID, uint8_t leaveCode, string reason);
    NodeID oldID() const;
    bool normal() const;
    bool kicked() const;
    bool banned() const;
    string reason() const;
    static const uint8_t NORMAL; /**< left normally code */
    static const uint8_t KICKED; /**< kicked code */
    static const uint8_t BANNED; /**< banned code */
  };
  /** \brief the packet sent from a server to clients indicating that the
   *         server has shut down
   */
  class Shutdown : public Packet
  {
  public:
    using Packet::populate;
    uint8_t type() const { return 8; }
    uint8_t size() const { return 0; }
  };
}
#endif
