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
 * File:    Bounds.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef  BOUNDS_H
#define  BOUNDS_H
#include "Pair.h"
namespace wic
{
  /** A bounding rectangle. Bounds consists of a lower-left and an upper-right
   *  coordinate.
   */
  class Bounds
  {
  public:
    /** Constructor.
     *  \param lowerLeft the lower-left coordinate
     *  \param upperRight the upper-right coordinate
     */
    Bounds(Pair lowerLeft, Pair upperRight);
    /** Constructor.
     *  \param lowerLeftX the x value of the lower-left coordinate
     *  \param lowerLeftY the y value of the lower-left coordinate
     *  \param upperRightX the x value of the upper-right coordinate
     *  \param upperRightY the y value of the upper-right coordinate
     */
    Bounds(int lowerLeftX, int lowerLeftY, int upperRightX, int upperRightY);
    /** Constructor.
     *  \param location the location of the lower-left coordinate
     *                  relative to whatever is being bounded
     *  \param xDimension the bounding width
     *  \param yDimension the bounding height
     */
    Bounds(Pair location, int xDimension, int yDimension);
    /** Default constructor (32x32). */
    Bounds();
    /** Copy constructor. */
    Bounds(const Bounds& other);
    Pair lowerLeft;  /**< Lower-left coordinate. */
    Pair upperRight; /**< Upper-right coordinate. */
  };
  
}
#endif
