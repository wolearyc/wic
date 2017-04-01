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
 * File:    Packet.cpp
 * ----------------------------------------------------------------------------
 */
#include "Packet.h"
namespace wic
{
  const uint8_t NAME_SIZE = 20;
  const NodeID SERVER_ID = 0;
  
  AbstractPacket::AbstractPacket()
  {
  }
  vector<uint8_t> AbstractPacket::data() const
  {
    return data_;
  }
  NodeID AbstractPacket::source() const
  {
    return source_;
  }
  void AbstractPacket::toBuffer(uint8_t* dest, NodeID source) const
  {
    if(!dest)
      throw InvalidArgument("dest", "should not be null");
    dest[0] = type();
    dest[1] = source;
    dest[2] = size();
    memcpy(dest + HEADER_SIZE, data_.data(), size());
  }

  const size_t AbstractPacket::HEADER_SIZE = sizeof(NodeID) + 2*sizeof(uint8_t);

  template<class SubPacket> Packet<SubPacket>::Packet(const AbstractPacket& other)
  {
    data_ = other.data();
    source_ = other.source();
  }
  
  template<class SubPacket> uint8_t Packet<SubPacket>::type() const
  {
    return SubPacket::Type;
  }
  template<class SubPacket> uint8_t Packet<SubPacket>::size() const
  {
    return SubPacket::Size;
  }

  void MysteryPacket::populate(uint8_t* src)
  {
    if(!src)
      throw InvalidArgument("src", "should not be null");
    type_ = src[0];
    source_ = src[1];
    size_ = src[2];
    data_.reserve(size_);
    for(uint8_t i=AbstractPacket::HEADER_SIZE; i < AbstractPacket::HEADER_SIZE + size_; i++)
      data_.push_back(src[i]);
  }
  void MysteryPacket::populate(const AbstractPacket& other)
  {
    type_ = other.type();
    source_ = other.source();
    size_ = other.size();
    data_ = other.data();
  }

  uint8_t MysteryPacket::type() const { return type_; }
  uint8_t MysteryPacket::size() const { return size_; }
  template <class PacketClass> bool MysteryPacket::isType() const
  {
    return type() == PacketClass::type;
  }
  
  JoinRequest::JoinRequest(string name)
  {
    data_.reserve(size());
    memcpy(data_.data(), name.data(), name.length() + 1);
  }
  string JoinRequest::name()  { return string((char*) data_.data()); }
  JoinResponse::JoinResponse(uint8_t responseCode, uint8_t numNodes,
                              NodeID assignedID, string serverName)
  {
    data_.reserve(size());
    data_[0] = responseCode;
    data_[1] = numNodes;
    data_[2] = assignedID;
    memcpy(&data_.data()[3], serverName.data(), serverName.length() + 1);
  }
  bool JoinResponse::ok() const           { return (data_[0] == OK); }
  bool JoinResponse::full() const         { return (data_[0] == FULL); }
  bool JoinResponse::banned() const       { return (data_[0] == BANNED); }
  uint8_t JoinResponse::maxNodes() const  { return data_[1]; }
  NodeID JoinResponse::assignedID() const { return data_[2]; }
  string JoinResponse::serverName() const { return string((char*) &data_.data()[3]); }
  const uint8_t JoinResponse::OK = 0;
  const uint8_t JoinResponse::FULL = 1;
  const uint8_t JoinResponse::BANNED = 2;
  
  ClientJoined::ClientJoined(NodeID newID, string newName)
  {
    data_.reserve(size());
    data_[0] = newID;
    memcpy(&data_.data()[1], newName.data(), newName.length() + 1);
  }
  NodeID ClientJoined::newID() const   { return data_[0]; }
  string ClientJoined::newName() const { return string((char*) &data_.data()[1]); }
  
  ClientInfo::ClientInfo(NodeID ID, string name)
  {
    data_.reserve(size());
    data_[0] = ID;
    memcpy(&data_.data()[1], name.data(), name.length() + 1);
  }
  NodeID ClientInfo::ID() const   { return data_[0]; }
  string ClientInfo::name() const { return string((char*) &data_.data()[1]); }
  
  Kick::Kick(string reason)
  {
    data_.reserve(size());
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  string Kick::reason() const  { return string((char*) data_.data()); }
  
  Ban::Ban(string reason)
  {
    data_.reserve(size());
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  string Ban::reason() const  { return string((char*) data_.data()); }
  
  ClientLeft::ClientLeft(NodeID oldID, uint8_t leaveCode, string reason)
  {
    data_.reserve(size());
    data_[0] = oldID;
    data_[1] = leaveCode;
    memcpy(&data_.data()[2], reason.data(), reason.length() + 1);
  }
  NodeID ClientLeft::oldID() const  { return data_[0]; }
  bool ClientLeft::normal() const   { return (data_[1] == NORMAL); }
  bool ClientLeft::kicked() const   { return (data_[1] == KICKED); }
  bool ClientLeft::banned() const   { return (data_[1] == BANNED); }
  string ClientLeft::reason() const { return string((char*) &data_.data()[2]); }
  const uint8_t ClientLeft::NORMAL = 0;
  const uint8_t ClientLeft::KICKED = 1;
  const uint8_t ClientLeft::BANNED = 2;
  
  Shutdown::Shutdown() {}
}
