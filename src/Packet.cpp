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
  const size_t AbstractPacket::HEADER_SIZE = 3*sizeof(uint8_t);
  AbstractPacket::AbstractPacket()
  {
  }
  vector<uint8_t> AbstractPacket::getData() const
  {
    return data;
  }
  NodeID AbstractPacket::getSource() const
  {
    return source;
  }
  void AbstractPacket::toBuffer(uint8_t* dest, NodeID source) const
  {
    if(dest == nullptr)
      throw InvalidArgument("dest", "null");
    dest[0] = getType();
    dest[1] = source;
    dest[2] = getSize();
    memcpy(dest + HEADER_SIZE, &data[0], getSize());
  }
  void MysteryPacket::populate(uint8_t* src)
  {
    if(src == nullptr)
      throw InvalidArgument("src", "null");
    type_ = src[0];
    source = src[1];
    size_ = src[2];
    data.resize(size_);
    memcpy(&data[0], &src[3], size_);
  }
  void MysteryPacket::populate(const AbstractPacket& other)
  {
    type_ = other.getType();
    source = other.getSource();
    size_ = other.getSize();
    data = other.getData();
  }
  uint8_t MysteryPacket::getType() const { return type_; }
  uint8_t MysteryPacket::getSize() const { return size_; }
  
  JoinRequest::JoinRequest(string name)
  {
    memcpy(&data[0], name.data(), name.size()+1);
  }
  string JoinRequest::name()  { return string((char*) data.data()); }
  
  JoinResponse::JoinResponse(uint8_t responseCode, NodeID maxID,
                              NodeID assignedID, string serverName)
  {
    data[0] = responseCode;
    data[1] = maxID;
    data[2] = assignedID;
    memcpy(&data[3], serverName.data(), serverName.size()+1);
  }
  bool JoinResponse::ok() const           { return (data[0] == OK); }
  bool JoinResponse::full() const         { return (data[0] == FULL); }
  bool JoinResponse::banned() const       { return (data[0] == BANNED); }
  uint8_t JoinResponse::maxID() const     { return data[1]; }
  NodeID JoinResponse::assignedID() const { return data[2]; }
  string JoinResponse::serverName() const { return string((char*) &data[3]); }
  const uint8_t JoinResponse::OK = 0;
  const uint8_t JoinResponse::FULL = 1;
  const uint8_t JoinResponse::BANNED = 2;
  
  ClientJoined::ClientJoined(NodeID newID, string newName)
  {
    data[0] = newID;
    memcpy(&data[1], newName.data(), newName.size()+1);
  }
  NodeID ClientJoined::newID() const   { return data[0]; }
  string ClientJoined::newName() const { return string((char*) &data[1]); }
  
  ClientInfo::ClientInfo(NodeID ID, string name)
  {
    data[0] = ID;
    memcpy(&data[1], name.data(), name.size()+1);
  }
  NodeID ClientInfo::ID() const   { return data[0]; }
  string ClientInfo::name() const { return string((char*) &data[1]); }
  
  Leaving::Leaving()
  {
  }
  
  Kick::Kick(string reason)
  {
    memcpy(&data[0], reason.data(), reason.size()+1);
  }
  string Kick::reason() const  { return string((char*) &data[0]); }
  
  Ban::Ban(string reason)
  {
    memcpy(&data[0], reason.data(), reason.size() + 1);
  }
  string Ban::reason() const  { return string((char*) data.data()); }
  
  ClientLeft::ClientLeft(NodeID oldID, uint8_t leaveCode, string reason)
  {
    data[0] = oldID;
    data[1] = leaveCode;
    memcpy(&data[2], reason.data(), reason.size()+1);
  }
  NodeID ClientLeft::oldID() const  { return data[0]; }
  bool ClientLeft::normal() const   { return (data[1] == NORMAL); }
  bool ClientLeft::kicked() const   { return (data[1] == KICKED); }
  bool ClientLeft::banned() const   { return (data[1] == BANNED); }
  string ClientLeft::reason() const { return string((char*) &data.data()[2]); }
  const uint8_t ClientLeft::NORMAL = 0;
  const uint8_t ClientLeft::KICKED = 1;
  const uint8_t ClientLeft::BANNED = 2;
  
  Shutdown::Shutdown()
  {
  }
}
