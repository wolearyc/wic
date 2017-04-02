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
 * File:    Quad.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef QUAD_H
#define QUAD_H
#include "Polygon.h"
namespace wic
{
  /** A filled rectangle. */
  class Quad
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Drawable
  {
  public:
    /** Constructor.
     *  \param location the screen location
     *  \param dimensions the dimensions
     *  \param color the desired color
     */
    Quad(Pair location, Pair dimensions, Color color);
    /** Default constructor. */
    Quad();
    /** Copy constructor. */
    Quad(const Quad& other);
    void draw(const Game& game);
    /** Returns the geometric center. */
    Pair getGeoCenter() const;
    Pair dimensions;      /**< the dimensions */
  };
}
#endif
