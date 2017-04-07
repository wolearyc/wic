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
 * File:    Font.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef FONT_H
#define FONT_H
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include "Game.h"
#include "Texture.h"
#include "Image.h"
#include "Error.h"
namespace wic
{
  /** A font. */
  class Font
  {
    friend class Text;
  public:
    /** Constructor.
     *  \param filepath the absolute or relative filepath to a TrueType (TTF),
     *         TrueType collection (TTC), Type 1 (PFA and PFB), CID-keyed Type 1,
     *         CFF, OpenType, OpenType collection, SFNT-based bitmap, X11 PCF,
     *         Windows FNT, BDF, or PFR font file
     * \param point the point of the font; must be nonzero
     * \param antialias whether or not to antialias. Small, antialiased fonts
     *        can be difficult to read.
     */
    Font(string filepath, unsigned point, bool antialias);
    /** Default constructor */
    Font();
    ~Font();
  private:
    const static unsigned NUM_CHARS = 128;
    FT_Face face;
    Texture textures[NUM_CHARS];
    unsigned short point;
    bool antialias;
  };
}
#endif
