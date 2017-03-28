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
  /** \brief a filled rectangle that can be drawn to the screen */
  class Quad
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Drawable
  {
  public:
    /** \brief Constructor
     *  \param location_ the desired screen location
     *  \param dimensions_ the dimensions
     *  \param color_ the desired color
     */
    Quad(Pair location_, Pair dimensions_, Color color_);
    /** \brief Default constructor */
    Quad();
    /** \brief Copy constructor */
    Quad(const Quad& other);
    void draw(const Game& game);
    /** \brief Gets the geometric center of the quad */
    Pair getGeoCenter() const;
    Pair dimensions;      /**< the dimensions */
  };
}
#endif
