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
 * File:    Bounds.cpp
 * ----------------------------------------------------------------------------
 */
/** \file */
#include "Bounds.h"
namespace wic
{
  Bounds::Bounds(Pair lowerLeft_, Pair upperRight_)
  :lowerLeft(lowerLeft_), upperRight(upperRight_)
  {
  }
  Bounds::Bounds(int lowerLeftX, int lowerLeftY, int upperRightX,
                 int upperRightY)
  :Bounds(Pair(lowerLeftX, lowerLeftY), Pair(upperRightX, upperRightY))
  {
  }
  Bounds::Bounds(Pair location, int xDimension, int yDimension)
  :Bounds(location, location + Pair(xDimension,yDimension))
  {
  }
  Bounds::Bounds(const Bounds& other)
  :Bounds(other.lowerLeft, other.upperRight)
  {
  }
  Bounds::Bounds()
  :Bounds(Pair(), Pair(32,32))
  {
  }
}
