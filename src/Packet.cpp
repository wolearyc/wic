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
  
  void Packet::populate(const Packet& other)
  {
    if(!isType(other))
      throw Error("packet ID mismatch");
    data_ = other.data_;
    source_ = other.source_;
  }
  vector<uint8_t> Packet::data() const
  {
    return data_;
  }
  NodeID Packet::source() const
  {
    return source_;
  }
  bool Packet::isType(const Packet& other) const
  {
    return type() == other.type();
  }
  void Packet::toBuffer(uint8_t* dest, NodeID source) const
  {
    if(!dest)
      throw InvalidArgument("dest", "should not be null");
    dest[0] = type();
    dest[1] = source;
    dest[2] = size();
    memcpy(dest + HEADER_SIZE, data_.data(), size());
  }
  const size_t Packet::HEADER_SIZE = sizeof(NodeID) + 2*sizeof(uint8_t);
  
  uint8_t MysteryPacket::type() const { return type_; }
  uint8_t MysteryPacket::size() const { return size_; }
  void MysteryPacket::populate(uint8_t* src)
  {
    if(!src)
      throw InvalidArgument("src", "should not be null");
    type_ = src[0];
    source_ = src[1];
    size_ = src[2];
    data_.reserve(size_);
    for(uint8_t i=Packet::HEADER_SIZE; i < Packet::HEADER_SIZE + size_; i++)
      data_.push_back(src[i]);
  }

  void JoinRequest::populate(string name)
  {
    data_.reserve(size());
    memcpy(data_.data(), name.data(), name.length() + 1);
  }
  string JoinRequest::name()  { return string((char*) data_.data()); }
  void JoinResponse::populate(uint8_t responseCode, uint8_t numNodes,
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
  
  void ClientJoined::populate(NodeID newID, string newName)
  {
    data_.reserve(size());
    data_[0] = newID;
    memcpy(&data_.data()[1], newName.data(), newName.length() + 1);
  }
  NodeID ClientJoined::newID() const   { return data_[0]; }
  string ClientJoined::newName() const { return string((char*) &data_.data()[1]); }
  
  void ClientInfo::populate(NodeID ID, string name)
  {
    data_.reserve(size());
    data_[0] = ID;
    memcpy(&data_.data()[1], name.data(), name.length() + 1);
  }
  NodeID ClientInfo::ID() const   { return data_[0]; }
  string ClientInfo::name() const { return string((char*) &data_.data()[1]); }
  
  void Kick::populate(string reason)
  {
    data_.reserve(size());
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  string Kick::reason() const  { return string((char*) data_.data()); }
  
  void Ban::populate(string reason)
  {
    data_.reserve(size());
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  string Ban::reason() const  { return string((char*) data_.data()); }
  
  void ClientLeft::populate(NodeID oldID, uint8_t leaveCode, string reason)
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
}
