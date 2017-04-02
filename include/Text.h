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
 * File:    Text.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef TEXT_H
#define TEXT_H
#include <vector>
#include "Font.h"
#include "Image.h"
using std::string;
using std::vector;
namespace wic
{
  /** Horizontal text. */
  class Text
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Bounded, public Drawable
  {
  public:
    /** Constructor.
     *  \param location the screen location
     *  \param str the string to render
     *  \param font the font
     *  \param color the color
     */
    Text(Pair location, string str, const Font* font, Color color);
    /** Default constructor. */
    Text();
    /** Copy constructor. */
    Text(const Text& other);
    ~Text();
    void draw(const Game& game);
    /** Sets the string to render and sets the bounds to display the entire
     *  string.
     *  \param str the new string to render
     */
    void setString(string str);
    /** Returns the current rendered string. */
    string getString() const;
  private:
    Bounds update();        // Updates offsets and returns new overall bounds.
    string str;             
    vector<Pair> offsets;   // the offsets of each glyph from location
    vector<Image> images;   // the glyph images
    const Font* font;
  };
}
#endif
