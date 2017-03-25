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
 * File:    Image.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef IMAGE_H
#define IMAGE_H
#include "Locateable.h"
#include "Rotateable.h"
#include "Scaleable.h"
#include "Colorable.h"
#include "Bounded.h"
#include "Drawable.h"
#include "Texture.h"
namespace wic
{
  /** \brief an image that can be drawn to the screen */
  class Image
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Bounded, public Drawable
  {
  public:
    /** \brief Constructor
     *  \param location_ the desired screen location
     *  \param texture_ the desired Texture
     */
    Image(Pair location_, Texture* texture_);
    /** \brief Default constructor */
    Image();
    /** \brief Copy constructor */
    Image(const Image& other);
    /** \brief fetches the geometric center of a Image
     *  \return the geometric center of the Image on success, {-1, -1} on failure
     */
    Pair getGeoCenter();
    /** \brief draws a Image to the screen
     *  \return true on success, false on failure
     */
    void draw(Game& game);
    /** \brief deallocates a Image
     *  \param target the target Image
     *  \return true on success, false on failure
     */
    Texture* texture;     /**< the texture */
  };
}
#endif
