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
 * File:    Hitbox.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef HITBOX_H
#define HITBOX_H
#include "Interfaces.h"
namespace wic
{
  class Contact
  {
  public:
    Contact(Pair point, double depth, Pair normal);
    Contact();
    bool isIntersecting() const;
    Pair getPoint() const;
    double getDepth() const;
    Pair getNormal() const;
  private:
    bool intersecting;
    Pair point;
    double depth;
    Pair normal;
  };
  class HitBoxAligned;
  class HitCircle;
  
  class HitBoxAligned
  : public Locateable, public Scaleable
  {
  public:
    HitBoxAligned(Pair location, Pair dimensions);
    HitBoxAligned();
    HitBoxAligned(const HitBoxAligned& other);
    bool isIntersecting(const HitBoxAligned& other) const;
    bool isIntersecting(const HitCircle& other) const;
    Contact getContact(const HitBoxAligned& other) const;
    Contact getContact(const HitCircle& other) const;
    Pair getGeoCenter() const;
    Pair getMin() const;
    Pair getMax() const;
    bool centered;
    Pair dimensions;
  };
  
  class HitCircle
  : public Locateable, public Scaleable
  {
  public:
    HitCircle(Pair location, double radius);
    HitCircle();
    HitCircle(const HitCircle& other);
    bool isIntersecting(const HitBoxAligned& other) const;
    bool isIntersecting(const HitCircle& other) const;
    Contact getContact(const HitBoxAligned& other) const;
    Contact getContact(const HitCircle& other) const;
    Pair getGeoCenter() const;
    bool centered;
    double radius;
  };
}
#endif
