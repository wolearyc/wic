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
#include <stdint.h>
#include "Game.h"
#include "Pair.h"
#include "Error.h"
namespace wic
{
  /** Enumerates methods of texture filtering. Texture filtering defines the
   *  behavior of a texture whe it scaled beyond or below its original 
   *  resolution.
   */
  enum class Filter
  {
    Nearest = GL_NEAREST, /**< nearest filtering (no filtering) */
    Linear = GL_LINEAR    /**< linear filtering */
  };
  /** Enumerates buffer formats. */
  enum class Format
  {
    Mono,      /**< each pixel has one 1 bit value (black or white) */
    Grayscale, /**< each pixel has one 8 bit value */
    RGB,       /**< each pixel has three 8 bit values: red, green, blue */
    RGBA       /**< each pixel has four 8 bit values: red, green, blue, alpha */
  };
  /** Enumerates texture wrapping behavior. Textures must be wrapped when drawn
   *  beyond their original dimensions.
   */
  enum class Wrap
  {
    Repeat = GL_REPEAT,               /**< Repeat texture. */
    ClampToEdge = GL_CLAMP_TO_EDGE,   /**< Smear edge pixels. */
    None = GL_CLAMP_TO_BORDER,        /**< Draw nothing. */
  };
  /** A texture. */
  class Texture
  {
    friend class Image;
  public:
    /** Constructur (from buffer).
     *  \param buffer a buffer
     *  \param dimensions the dimensions; must be compatible with buffer and
     *         format
     *  \param format the buffer format
     *  \param filter the texture filter
     *  \param wrap the texture wrapping
     */
    Texture(vector<uint8_t> buffer, Pair dimensions, enum Format format,
            enum Filter filter, enum Wrap wrap);
    /** Constructor (from image file).
     *  \param filepath an absolute or relative filepath to a non-1bpp and 
     *         non-RLE, BMP, non-interlaced PNG, JPEG, TGA, DDS, PSD, or HDR file
     *  \param filter the texture filter
     *  \param wrap the texture wrapping
     */
    Texture(string filepath, enum Filter filter, enum Wrap wrap);
    /** Constructor (image file, nearest filter and no wrapping)
     *  \param filepath an absolute or relative filepath to a non-1bpp and
     *         non-RLE, BMP, non-interlaced PNG, JPEG, TGA, DDS, PSD, or HDR file
     */
    Texture(string filepath);
    /** Copy constructor. */
    Texture(const Texture& other);
    /** Default constructor. */
    Texture();
    ~Texture();
    /** Loads the texture into the GPU, enabling drawing. */
    void load();
    /** Returns the dimensions. */
    Pair getDimensions() const;
  private:
    void init(vector<uint8_t> buffer, Pair dimensions, enum Format format,
              enum Filter filter, enum Wrap wrap);
    unsigned data;
    Pair dimensions;
    bool loaded;
    vector<uint8_t> formattedBuffer;
    enum Filter filter;
    enum Wrap wrap;
  };
}
#endif
