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
  /** \brief horizontal text that can be drawn to the screen */
  class Text
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Bounded, public Drawable
  {
  public:
    /** \brief Constructor
     *  \param location_ the desired screen location
     *  \param str the desired string to render
     *  \param font the desired font
     *  \param color_ the desired color
     */
    Text(Pair location_, string str, const Font* font, Color color_);
    /** \brief Default constructor */
    Text();
    /** \brief Copy constructor */
    Text(const Text& other);
    ~Text();
    void draw(const Game& game);
    /** \brief sets the string to render
     *  \param str the new string to render
     */
    void setString(string str);
    /** \brief gets the current string */
    string getString() const;
  private:
    Bounds getData();
    string str_;             
    vector<Pair> offsets_;   // the offsets of each glyph from location
    vector<Image> images_;   // the glyph images
    const Font* font_;
  };
}
#endif
