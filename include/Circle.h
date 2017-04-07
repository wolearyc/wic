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
 * File:    Circle.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef CIRCLE_H
#define CIRCLE_H
#include "Interfaces.h"
namespace wic
{
  /** A filled circle. */
  class Circle
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Drawable
  {
  public:
    /** Constructor.
     *  \param location the screen location
     *  \param radius the radius
     *  \param color the color
     */
    Circle(Pair location, unsigned radius, Color color);
    /** Default constructor. */
    Circle();
    /** Copy constructor. */
    Circle(const Circle& other);
    void draw();
    Pair getGeoCenter() const;
    unsigned radius;      /**< the radius */
  };
}
#endif
