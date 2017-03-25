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
 * File:    Text.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef TEXT_H
#define TEXT_H
#include <vector>
#include "Locateable.h"
#include "Rotateable.h"
#include "Scaleable.h"
#include "Colorable.h"
#include "Drawable.h"
#include "Font.h"
#include "Image.h"
using std::string;
using std::vector;
namespace wic
{
  /** \brief horizontal text that can be drawn to the screen
   */
  class Text
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Bounded, public Drawable
  {
  public:
    /** \brief Constructor
     *  \param location_ the desired screen location
     *  \param str the desired string to draw
     *  \param font the desired font
     *  \param color_ the desired color
     */
    Text(Pair location_, string str, Font* font, Color color_);
    /** \brief Default constructor */
    Text();
    /** \brief Copy constructor */
    Text(const Text& other);
    ~Text();
    void draw(Game& game);
    void setString(string str);
    string getString();
  private:
    Bounds getData();
    string str_;             /**< the string to draw */
    vector<Pair> offsets_;   /**< the offsets of each glyph from location */
    vector<Image> images_;   /**< the glyph images */
    Font* font_;             /**< the font */
  };
}
#endif
