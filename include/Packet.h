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
#include "Node.h"
using std::string;
using std::vector;
namespace wic
{
  /** Abstract network packet. A network packet has a source (ID of node who
   *  sent the packet), a type (identifier for the type of data carried), a 
   *  size (the amount of data carried), and finally a data payload.
   */
  class AbstractPacket
  {
  public:
    const static size_t HEADER_SIZE; /**< Size of the packet's header */
    /** Default constructor. */
    AbstractPacket();
    /** Returns the data payload. */
    vector<uint8_t> getData() const;
    /** Returns the ID of the sender. */
    NodeID getSource() const;
    /** Populates a buffer to send over the network.
     *  \param dest destination buffer
     *  \param source the desired source of the packet
     */
    void toBuffer(uint8_t* dest, NodeID source) const;
    /** Returns the type. */
    virtual uint8_t getType() const = 0;
    /** Returns the size. */
    virtual uint8_t getSize() const = 0;
  protected:
    vector<uint8_t> data;
    NodeID source;
  };
  /** Concrete packet of a specific type. Specific packets are subclasses of
   *  Packet. Subclasses should define two static, constant variables: TYPE
   *  and SIZE.
   */
  template <class Subclass> class Packet : public AbstractPacket
  {
  public:
    /** Constructor. This constructor is used to convert MysteryPackets into
     *  concrete packets. The constructor performs a check to make sure the
     *  conversion is valid.
     */
    Packet(const AbstractPacket& other)
    {
      data = other.getData();
      source = other.getSource();
    }
    /** Default constructor. */
    Packet()
    {
    }
    uint8_t getType() const
    {
      return Subclass::TYPE;
    }
    uint8_t getSize() const
    {
      return Subclass::SIZE;
    }
  };
  
  /** Packet of undetermined type. MysteryPackets are recieved. */
  class MysteryPacket : public AbstractPacket
  {
  public:
    using AbstractPacket::AbstractPacket;
    /** Populates the packet from a network buffer
     *  \param src a network buffer
     */
    void populate(uint8_t* src);
    /** Populates the packet from another packet
     *  \param other another packet
     */
    void populate(const AbstractPacket& other);
    uint8_t getType() const;
    uint8_t getSize() const;
    /** Returns whether or not the mystery packet is of the same type as a 
     *  concrete packet.
     */
    template <class PacketClass> bool isType() const
    {
      return getType() == PacketClass::TYPE;
    }
  private:
    uint8_t type_;
    uint8_t size_;
  };
  
  /** Packet a client sends to a server when requesting to join. */
  class JoinRequest : public Packet<JoinRequest>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param name client name
     */
    JoinRequest(string name);
    static const uint8_t TYPE = 0;
    static const uint8_t SIZE = 21;
    /** Returns the client's name */
    string name();
  };
  
  /** The packet a server sends to a client in response to a join request. */
  class JoinResponse : public Packet<JoinResponse>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param responseCode response code (OK, FULL, or BANNED)
     *  \param maxID maximum ID of the server
     *  \param assignedID ID assigned to the new client
     *  \param serverName server name
     */
    JoinResponse(uint8_t responseCode, NodeID maxID, NodeID assignedID,
                 string serverName);
    static const uint8_t TYPE = 1;
    static const uint8_t SIZE = 24;
    /** Returns whether or not join is ok. */
    bool ok() const;
    /** Returns whether or not join failed due to a full server. */
    bool full() const;
    /** Returns whether or not join failed because the client is banned. */
    bool banned() const;
    /** Returns the maximum ID the server supports. */
    uint8_t maxID() const;
    /** Returns the new ID assigned to the client. */
    NodeID assignedID() const;
    /** Returns the server's name. */
    string serverName() const;
    static const uint8_t OK;     /**< successful join  code */
    static const uint8_t FULL;   /**< failed join code due to full server */
    static const uint8_t BANNED; /**< failed join code due to client ban */
  };
  /** The packet recieved by all nodes announcing a new client. */
  class ClientJoined : public Packet<ClientJoined>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param newID ID of the new client
     *  \param newName name of the new client
     */
    ClientJoined(NodeID newID, string newName);
    static const uint8_t TYPE = 2;
    static const uint8_t SIZE = 22;
    /** Returns the new client's ID. */
    NodeID newID() const;
    /** Returns the new client's name. */
    string newName() const;
  };
  /** The packet sent from a server to a new client notifying them of an
   *  existing client. 
   */
  class ClientInfo : public Packet<ClientInfo>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param ID ID of existing client
     *  \param name name of existing client
     */
    ClientInfo(NodeID ID, string name);
    static const uint8_t TYPE = 3;
    static const uint8_t SIZE = 22;
    /** Returns the existing client's ID. */
    NodeID ID() const;
    /** Returns the existing client's name. */
    string name() const;
  };
  /** Packet sent from a client to a server when the client leaves. */
  class Leaving : public Packet<Leaving>
  {
  public:
    using Packet::Packet;
    /** Default constructor. */
    Leaving();
    static const uint8_t TYPE = 4;
    static const uint8_t SIZE = 0;
  };
  /** Packet sent from a server to a client that kicks the client. */
  class Kick : public Packet<Kick>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param reason reason for the kick
     */
    Kick(string reason);
    static const uint8_t TYPE = 5;
    static const uint8_t SIZE = 51;
    /** Returns the reason for the kick. */
    string reason() const;
  };
  /** Packet sent from a server to a client that bans the client. */
  class Ban : public Packet<Ban>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param reason reason for the ban
     */
    Ban(string reason);
    static const uint8_t TYPE = 6;
    static const uint8_t SIZE = 51;
    /** Returns the reason for the ban. */
    string reason() const;
  };
  /** Packet sent from a server to all connected clients when a client leaves */
  class ClientLeft : public Packet<ClientLeft>
  {
  public:
    using Packet::Packet;
    /** Constructor.
     *  \param oldID ID of old client
     *  \param leaveCode code indicating leave condition
     *  \param reason reason for ban/kick (if applicable)
     */
    ClientLeft(NodeID oldID, uint8_t leaveCode, string reason);
    static const uint8_t TYPE = 7;
    static const uint8_t SIZE = 53;
    /** Returns ID of old client. */
    NodeID oldID() const;
    /** Returns whether or not the client left normally. */
    bool normal() const;
    /** Returns whether or not the client left due to kick. */
    bool kicked() const;
    /** Returns whether or not the client left due to ban. */
    bool banned() const;
    /** Returns the reason for ban/kick (if applicable) */
    string reason() const;
    static const uint8_t NORMAL; /**< left normally code */
    static const uint8_t KICKED; /**< kicked code */
    static const uint8_t BANNED; /**< banned code */
  };
  /** Packet sent from a server to connected clients indicating server
   *  shutdown.
   */
  class Shutdown : public Packet<Shutdown>
  {
  public:
    using Packet::Packet;
    /** Default constructor. */
    Shutdown();
    static const uint8_t TYPE = 8;
    static const uint8_t SIZE = 0; 
  };
  
}
#endif
