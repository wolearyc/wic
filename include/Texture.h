/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for Mac OSX written in C++
 * Copyright (C) 2013-2017  Willis O'Leary
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.s
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 * File:    Texture.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef TEXTURE_H
#define TEXTURE_H
#include "stdlib.h"
#include "Pair.h"
#include "Error.h"
#include <SOIL/SOIL.h>
#include <OpenGL/gl.h>
namespace wic
{
  /** \brief defines constants for texture filtering (behavior when images are
   *         scaled beyond or below their resolution)
   */
  enum WicFilter
  {
    WIC_NEAREST = GL_NEAREST, /**< nearest filtering (no filtering) */
    WIC_LINEAR = GL_LINEAR    /**< linear filtering */
  };
  /** \brief defines constants for buffer formats
   */
  enum WicFormat
  {
    WIC_MONO,      /**< each pixel has one 1 bit value (black or white) */
    WIC_GREYSCALE, /**< each pixel has one 8 bit value */
    WIC_RGB,       /**< each pixel has three 8 bit values, red, green, and
                    *   blue
                    */
    WIC_RGBA       /**< each pixel has four 8 bit values, red, green, blue, and
                    *   alpha
                    */
  };
  /** \brief defines constants for texture wrapping (behavior when drawing
   *         outside of texture dimensions
   */
  enum WicWrap
  {
    WIC_REPEAT = GL_REPEAT,               /**< the texture repeats */
    WIC_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, /**< the edges pixels are drawn out */
    WIC_STOP = GL_CLAMP_TO_BORDER,        /**< nothing is drawn outside of the
                                           *   texture dimensions
                                           */
  };
  /** \brief a texture */
  class Texture
  {
    friend class Image;
  public:
    /** \brief initializes a Texture from an existing buffer
     *  \param buffer the buffer
     *  \param the dimensions; dimensions.x * dimensions.y not being equal to the
     *         number of elements in buffer will result in undefined behavior
     *  \param format the format of buffer; an incorrect format will result in a
     *         warped texture and/or undefined behavior
     *  \param filter the desired texture filter (defines behavior when textures are
     *         scaled beyond or below their resolution
     *  \param wrap the texture wrap (defines behavior when drawing outside texture
     *         dimensions
     *  \return a valid pointer to a Texture on success, null on failure
     */
    Texture(unsigned char* buffer, Pair dimensions, enum WicFormat format,
            enum WicFilter filter, enum WicWrap wrap);
    /** \brief initializes a Texture from a file
     *  \param filepath the absolute or relative filepath to a non-1bpp and non-RLE
     *         BMP, non-interlaced PNG, JPEG, TGA, DDS, PSD, or HDR image file; the
     *         file must exist and be one of the formentioned formats
     *  \param filter the desired texture filter (defines behavior when textures are
     *          scaled beyond or below their resolution
     *  \param wrap the texture wrap (defines behavior when drawing outside texture
     *         dimensions
     *  \return a valid pointer to a Texture on success, null on failure
     */
    Texture(string filepath, enum WicFilter filter, enum WicWrap wrap);
    /** \brief Default constructor */
    Texture();
    ~Texture();
    /** \brief returns the dimensions of a texture.
     *  \return the dimensons of target on success, (-1, -1) on failure.
     */
    Pair getDimensions();
  private:
    unsigned int data_;
    Pair dimensions_;
  };
}
#endif
