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
 * File:    Interfaces.cpp
 * ----------------------------------------------------------------------------
 */
/** \file */
#include "Interfaces.h"
namespace wic
{
  Locateable::Locateable(Pair location, Pair center)
  : location(location), center(center)
  {
  }
  Locateable::Locateable(Pair location)
  : location(location), center(Pair())
  {
  }
  Locateable::Locateable()
  : location(Pair()), center(Pair())
  {
  }
  Locateable::Locateable(const Locateable& other)
  : location(other.location), center(other.center)
  {
  }
  Rotateable::Rotateable(double rotation)
  : rotation(rotation)
  {
  }
  Rotateable::Rotateable()
  : rotation(0.0)
  {
  }
  Rotateable::Rotateable(const Rotateable& other)
  : rotation(other.rotation)
  {
  }
  Scaleable::Scaleable(Pair scale)
  : scale(scale)
  {
  }
  Scaleable::Scaleable()
  : scale(Pair(1,1))
  {
  }
  Scaleable::Scaleable(const Scaleable& other)
  : scale(other.scale)
  {
  }
  Colorable::Colorable(Color color)
  : color(color)
  {
  }
  Colorable::Colorable()
  : color(Color::White)
  {
  }
  Colorable::Colorable(const Colorable& other)
  : color(other.color)
  {
  }
  Bounded::Bounded(Bounds bounds)
  : bounds(bounds)
  {
  }
  Bounded::Bounded()
  : bounds(Bounds())
  {
  }
  Bounded::Bounded(const Bounded& other)
  : bounds(other.bounds)
  {
  }
  Drawable::Drawable(bool drawCentered)
  : drawCentered(drawCentered)
  {
  }
  Drawable::Drawable()
  : drawCentered(false)
  {
  }
  Drawable::Drawable(const Drawable& other)
  : drawCentered(other.drawCentered)
  {
  }
}
