/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for MacOS written in C++
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
#include <SOIL/SOIL.h>
#include <OpenGL/gl.h>
#include "Pair.h"
#include "Error.h"
namespace wic
{
  /** \brief defines constants for texture filtering (behavior when images are
   *         scaled beyond or below their resolution)
   */
  enum class Filter
  {
    Nearest = GL_NEAREST, /**< nearest filtering (no filtering) */
    Linear = GL_LINEAR    /**< linear filtering */
  };
  /** \brief defines constants for buffer formats
   */
  enum class Format
  {
    Mono,      /**< each pixel has one 1 bit value (black or white) */
    Grayscale, /**< each pixel has one 8 bit value */
    RGB,       /**< each pixel has three 8 bit values, red, green, and
                    *   blue
                    */
    RGBA       /**< each pixel has four 8 bit values, red, green, blue, and
                    *   alpha
                    */
  };
  /** \brief defines constants for texture wrapping (behavior when drawing
   *         outside of texture dimensions
   */
  enum class Wrap
  {
    Repeat = GL_REPEAT,               /**< the texture repeats */
    ClampToEdge = GL_CLAMP_TO_EDGE,   /**< the edges pixels are drawn out */
    Stop = GL_CLAMP_TO_BORDER,        /**< nothing is drawn outside of the
                                        *   texture dimensions
                                        */
  };
  /** \brief a texture */
  class Texture
  {
    friend class Image;
  public:
    /** \brief Constructur (from buffer)
     *  \param buffer the buffer
     *  \param dimensions the buffer dimensions
     *  \param format the buffer format
     *  \param filter the desired texture filter
     *  \param wrap the texture wrapping
     */
    Texture(unsigned char* buffer, Pair dimensions, enum Format format,
            enum Filter filter, enum Wrap wrap);
    /** \brief Constructor (from image file)
     *  \param filepath the absolute or relative filepath to a non-1bpp and non-RLE
     *         BMP, non-interlaced PNG, JPEG, TGA, DDS, PSD, or HDR image file
     *  \param filter the texture filter
     *  \param wrap the texture wrap
     */
    Texture(string filepath, enum Filter filter, enum Wrap wrap);
    /** \brief Default constructor */
    Texture();
    ~Texture();
    /** \brief returns the dimensions. */
    Pair getDimensions() const;
  private:
    unsigned int data_;
    Pair dimensions_;
  };
}
#endif
