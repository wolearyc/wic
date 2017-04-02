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
 * File:    Interfaces.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef INTERFACES_H
#define INTERFACES_H
#include "Pair.h"
#include "Color.h"
#include "Bounds.h"
#include "Game.h"
namespace wic
{
  /** An object possessing a location. Most objects are 2D, necessitating the 
   *  specification of a center about which the location is defined.
   */
  class Locateable
  {
  public:
    /** Constructor.
     *  \param location the location
     *  \param center the center
     */
    Locateable(Pair location, Pair center);
    /** Constructor.
     *  \param location the location
     */
    Locateable(Pair location);
    /** Default constructor, */
    Locateable();
    /** Copy constructor. */
    Locateable(const Locateable& other);
    Pair location;    /**< the location */
    Pair center;      /**< the center */
  };
  /** An object possessing a rotation. */
  class Rotateable
  {
  public:
    /** Constructor.
     *  \param rotation the rotation in radians from the positive x-axis
     */
    Rotateable(double rotation);
    /** Default constructor (0 rotation). */
    Rotateable();
    /** Copy constructor. */
    Rotateable(const Rotateable& other);
    double rotation;  /**< the rotation in radians */
  };
  /** An object possessing a 2D scale. */
  class Scaleable
  {
  public:
    /** Constructor.
     *  \param scale the scale
     */
    Scaleable(Pair scale);
    /** Default constructor (1x1 scale). */
    Scaleable();
    /** Copy constructor. */
    Scaleable(const Scaleable& other);
    Pair scale;    /**< the scale */
  };
  /** An object possessing a color. */
  class Colorable
  {
  public:
    /** Constructor.
     *  \param color the color
     */
    Colorable(Color color);
    /** Default constructor. */
    Colorable();
    /** Copy constructor. */
    Colorable(const Colorable& other);
    Color color; /**< the color */
  };
  /** An object possessing rectangular bounds. Bounds are used to draw only a 
   *  section of an object.
   */
  class Bounded
  {
  public:
    /** Constructor.
     *  \param bounds the bounds
     */
    Bounded(Bounds bounds);
    /** Default constructor. */
    Bounded();
    /** Copy constructor. */
    Bounded(const Bounded& other);
    Bounds bounds; /**< the bounds. */
  };
  /** An object that can be drawn to the screen. */
  class Drawable
  {
  public:
    /** Constructor.
     *  \param drawCentered whether or not to draw around some center
     */
    Drawable(bool drawCentered);
    /** Default constructor (not drawn centered). */
    Drawable();
    /** Copy constructor. */
    Drawable(const Drawable& other);
    /** Draws the object on the screen at its location.
     *  \param game the game
     */
    virtual void draw(const Game& game) = 0;
    /** Gets the geometric center. */
    virtual Pair getGeoCenter() const = 0;
    bool drawCentered; /**< whether or not to draw around some center */
  };
}
#endif
