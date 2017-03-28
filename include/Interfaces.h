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
  /** \brief An object possessing a location
   *
   *  Most objects are 2D, necessitating the specification of a center about
   *  which the location is defined.
   */
  class Locateable
  {
  public:
    /** \brief Constructor
     *  \param location_ the location
     *  \param center_ the center
     */
    Locateable(Pair location_, Pair center_);
    /** \brief Constructor
     *  \param location_ the location
     */
    Locateable(Pair location_);
    /** \brief Default constructor */
    Locateable();
    /** \brief Copy constructor */
    Locateable(const Locateable& other);
    Pair location;    /**< \brief the location */
    Pair center;      /**< \brief the center */
  };
  /** \brief An object possessing a rotation */
  class Rotateable
  {
  public:
    /** \brief Constructor
     *  \param rotation_ the rotation in radians from the positive x-axis
     */
    Rotateable(double rotation_);
    /** \brief Default constructor (0 rotation) */
    Rotateable();
    /** \brief Copy constructor */
    Rotateable(const Rotateable& other);
    double rotation;  /**< \brief the rotation in radians */
  };
  /** \brief An object possessing a 2D scale */
  class Scaleable
  {
  public:
    /** \brief Constructor
     *  \param scale_ the scale
     */
    Scaleable(Pair scale_);
    /** \brief Default constructor (1x1 scale) */
    Scaleable();
    /** \brief Copy constructor */
    Scaleable(const Scaleable& other);
    Pair scale;    /**< \brief the scale */
  };
  /** \brief An object possessing a color */
  class Colorable
  {
  public:
    /** \brief Constructor
     *  \param color_ the color
     */
    Colorable(Color color_);
    /** \brief Default constructor */
    Colorable();
    /** \brief Copy constructor */
    Colorable(const Colorable& other);
    Color color; /**< \brief the color */
  };
  /** \brief An object possessing rectangular bounds
   *
   *  The lower left and upper right coordinate typically defines the section
   *  to draw on the object.
   */
  class Bounded
  {
  public:
    /** \brief Constructor
     *  \param bounds_ the bounds
     */
    Bounded(Bounds bounds_);
    /** \brief Default constructor */
    Bounded();
    /** \brief Copy constructor */
    Bounded(const Bounded& other);
    Bounds bounds; /**< \brief the bounds */
  };
  /** \brief an object that can be drawn to the screen */
  class Drawable
  {
  public:
    /** \brief Constructor
     *  \param drawCentered_ whether or not to draw around some center
     */
    Drawable(bool drawCentered_);
    /** \brief Default constructor (not drawn centered) */
    Drawable();
    /** \brief Copy constructor */
    Drawable(const Drawable& other);
    /** \brief draws the object on the screen at its location
     *  \param game the game
     */
    virtual void draw(const Game& game) = 0;
    /** \brief gets the geometric center */
    virtual Pair getGeoCenter() const = 0;
    bool drawCentered; /**< whether or not to draw around some center */
  };
}
#endif
