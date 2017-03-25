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
 * File:    Packet.cpp
 * ----------------------------------------------------------------------------
 */
#include "Packet.h"
namespace wic
{
  const uint8_t NAME_SIZE = 20;
  const NodeID SERVER_ID = 0;
  
  Packet::Packet(uint8_t* buffer)
  {
    fromBuffer(buffer);
  }
  Packet::Packet()
  {
  }
  Packet::Packet(const Packet& other)
  :type_(other.type_), source_(other.source_), data_(other.data_),
   size_(other.size_)
  {
  }
  void Packet::fromBuffer(uint8_t* src)
  {
    if(!src)
      throw WicError(WIC_ERRNO_NULL_BUFFER);
    type_ = src[0];
    source_ = src[1];
    size_ = src[2];
    data_.reserve(size_);
    for(uint8_t i=Packet::HEADER_SIZE; i < Packet::HEADER_SIZE + size_; i++)
      data_.push_back(src[i]);
  }
  void Packet::toBuffer(uint8_t* dest)
  {
    if(!dest)
      throw WicError(WIC_ERRNO_NULL_BUFFER);
    dest[0] = type_;
    dest[1] = source_;
    dest[2] = size_;
    memcpy(dest + HEADER_SIZE, data_.data(), size_);
  }
  uint8_t Packet::type() { return type_; };
  uint8_t Packet::size() { return size_; };
  const size_t Packet::HEADER_SIZE = sizeof(NodeID) + 2*sizeof(uint8_t);
  
  
  JoinRequest::JoinRequest(string name)
  {
    data_.reserve(size);
    memcpy(data_.data(), name.data(), name.length() + 1);
  }
  JoinRequest::JoinRequest(const Packet& packet) : Packet(packet) {};
  string JoinRequest::name()  { return string((char*) data_.data()); }
  
  JoinResponse::JoinResponse(uint8_t responseCode, uint8_t numNodes,
                             NodeID assignedID, string serverName)
  {
    data_.reserve(size);
    data_[0] = responseCode;
    data_[1] = numNodes;
    data_[2] = assignedID;
    memcpy(&data_.data()[3], serverName.data(), serverName.length() + 1);
  }
  JoinResponse::JoinResponse(const Packet& packet) : Packet(packet) {};
  bool JoinResponse::joined()       { return (data_[0] == OK); }
  bool JoinResponse::full()         { return (data_[0] == FULL); }
  bool JoinResponse::banned()       { return (data_[0] == BANNED); }
  uint8_t JoinResponse::numNodes()  { return data_[1]; }
  NodeID JoinResponse::assignedID() { return data_[2]; }
  string JoinResponse::serverName() { return string((char*) &data_.data()[3]); }
  const uint8_t JoinResponse::OK = 0;
  const uint8_t JoinResponse::FULL = 1;
  const uint8_t JoinResponse::BANNED = 2;
  
  
  ClientJoined::ClientJoined(NodeID newID, string newName)
  {
    data_.reserve(size);
    data_[0] = newID;
    memcpy(&data_.data()[1], newName.data(), newName.length() + 1);
  }
  ClientJoined::ClientJoined(const Packet& packet) : Packet(packet) {};
  NodeID ClientJoined::newID()   { return data_[0]; }
  string ClientJoined::newName() { return string((char*) &data_.data()[1]); }
  
  
  ClientInfo::ClientInfo(NodeID ID, string name)
  {
    data_.reserve(size);
    data_[0] = ID;
    memcpy(&data_.data()[1], name.data(), name.length() + 1);
  }
  ClientInfo::ClientInfo(const Packet& packet) : Packet(packet) {};
  NodeID ClientInfo::ID()   { return data_[0]; }
  string ClientInfo::name() { return string((char*) &data_.data()[1]); }
  
  
  Leaving::Leaving() {};
  Leaving::Leaving(const Packet& packet) : Packet(packet) {};
  
  Kick::Kick(string reason)
  {
    data_.reserve(size);
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  Kick::Kick(const Packet& packet) : Packet(packet) {};
  string Kick::reason()  { return string((char*) data_.data()); }
  
  Ban::Ban(string reason)
  {
    data_.reserve(size);
    memcpy(data_.data(), reason.data(), reason.length() + 1);
  }
  Ban::Ban(const Packet& packet) : Packet(packet) {};
  string Ban::reason()  { return string((char*) data_.data()); }
  
  ClientLeft::ClientLeft(NodeID oldID, uint8_t leaveCode, string reason)
  {
    data_.reserve(size);
    data_[0] = oldID;
    data_[1] = leaveCode;
    memcpy(&data_.data()[2], reason.data(), reason.length() + 1);
  }
  ClientLeft::ClientLeft(const Packet& packet) : Packet(packet) {}
  NodeID ClientLeft::oldID()  { return data_[0]; }
  bool ClientLeft::normal()   { return (data_[1] == NORMAL); }
  bool ClientLeft::kicked()   { return (data_[1] == KICKED); }
  bool ClientLeft::banned()   { return (data_[1] == BANNED); }
  string ClientLeft::reason() { return string((char*) &data_.data()[2]); }
  const uint8_t ClientLeft::NORMAL = 0;
  const uint8_t ClientLeft::KICKED = 1;
  const uint8_t ClientLeft::BANNED = 2;
  
  Shutdown::Shutdown() {};
  Shutdown::Shutdown(const Packet& packet) : Packet(packet) {};
  
  bool wic_is_reserved_packet_id(uint8_t packet_id)
  {
    return packet_id <= 15;
  }
}
