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
 * File:    Locateable.cpp
 * ----------------------------------------------------------------------------
 */
/** \file */
#include "Locateable.h"
namespace wic
{
  Locateable::Locateable(Pair location_, Pair center_)
  : location(location_), center(center_)
  {
  }
  Locateable::Locateable(Pair location_)
  : location(location_), center(Pair())
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
}
