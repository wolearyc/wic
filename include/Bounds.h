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
 * File:    Bounds.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef  BOUNDS_H
#define  BOUNDS_H
#include "Pair.h"
namespace wic
{
  /** \brief a bounding rectangle, consisting of a lower left and an upper right
   *         coordinate
   *
   *  With an Image, the lower left coordinate defines what part of the Image to
   *  draw at the lower left, while the upper right coordinate defines what part
   *  of the Image to draw at the upper right. This mechanism can be used to
   *  flip the Image along some axis.
   */
  class Bounds
  {
  public:
    /** \brief constructs a Bounds object based on two coordinates
     *  \param lowerLeft_ the desired lower-left coordinate
     *  \param upperRight_ the desired upper-right coordinate
     */
    Bounds(Pair lowerLeft_, Pair upperRight_);
    /** \brief constructs a Bounds object based on the components of two
     *        coordinates
     *  \param lowerLeftX the desired x value of the lower-left coordinate
     *  \param lowerLeftY the desired y value of the lower-left coordinate
     *  \param upperRightX the desired x value of the upper-right coordinate
     *  \param upperRightY the desired y value of the upper-right coordinate
     */
    Bounds(int lowerLeftX, int lowerLeftY, int upperRightX, int upperRightY);
    /** \brief constructs a Bounds object based on a base location and
     *        dimensions
     *  \param location the desired location of the lower-left coordinate
     *         relative to whatever the Bounds is bounding
     *  \xDimension the desired width from the lower-left coordinate
     *  \yDimension the desired height from the lower-left coordinate
     */
    Bounds(Pair location, int xDimension, int yDimension);
    /** \brief constructs a Bounds object with a lower-left coordinate of
     *         (0,0) and an upper-right coordinate of (32,32)
     */
    Bounds();
    /** \brief contructs a Bounds object identical to another
     *  \param other another Bounds object
     */
    Bounds(const Bounds& other);
    Pair lowerLeft;  /**< lower left coordinate */
    Pair upperRight; /**< upper right coordinate */
  };
  
}
#endif
