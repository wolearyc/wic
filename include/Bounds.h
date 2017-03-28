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
  /** \brief a bounding rectangle, consisting of a lower left and an upper right
   *         coordinate
   */
  class Bounds
  {
  public:
    /** \brief Constructor
     *  \param lowerLeft_ the desired lower-left coordinate
     *  \param upperRight_ the desired upper-right coordinate
     */
    Bounds(Pair lowerLeft_, Pair upperRight_);
    /** \brief Constructor
     *  \param lowerLeftX the desired x value of the lower-left coordinate
     *  \param lowerLeftY the desired y value of the lower-left coordinate
     *  \param upperRightX the desired x value of the upper-right coordinate
     *  \param upperRightY the desired y value of the upper-right coordinate
     */
    Bounds(int lowerLeftX, int lowerLeftY,
           int upperRightX, int upperRightY);
    /** \brief Constructor
     *  \param location the desired location of the lower-left coordinate
     *         relative to whatever the Bounds is bounding
     *  \xDimension the desired width from the lower-left coordinate
     *  \yDimension the desired height from the lower-left coordinate
     */
    Bounds(Pair location, int xDimension, int yDimension);
    /** \brief Default constructor */
    Bounds();
    /** \brief Copy constructor */
    Bounds(const Bounds& other);
    Pair lowerLeft;  /**< \brief lower left coordinate */
    Pair upperRight; /**< \brief upper right coordinate */
  };
  
}
#endif
