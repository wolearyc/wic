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
 * File:    Image.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef IMAGE_H
#define IMAGE_H
#include "Interfaces.h"
#include "Texture.h"
namespace wic
{
  /** An image. */
  class Image
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Bounded, public Drawable
  {
  public:
    /** Constructor.
     *  \param location the screen location
     *  \param texture the texture
     */
    Image(Pair location, const Texture* texture);
    /** Default constructor. */
    Image();
    /** Copy constructor. */
    Image(const Image& other);
    /** Returns the geometric center. */
    Pair getGeoCenter() const;
    void draw();
    Bounds getWholeBounds();
    const Texture* texture;     /**< the texture */
  };
}
#endif
