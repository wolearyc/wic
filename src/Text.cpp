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
 * File:    Text.cpp
 * ----------------------------------------------------------------------------
 */
#include "Text.h"
namespace wic
{
  static Image working_image;
  Text::Text(Pair location, string str, const Font* font, Color color)
  : Locateable(location), Rotateable(), Scaleable(), Colorable(color),
    Bounded(), Drawable(), str(str), font(font)
  {
    if(font == nullptr)
      throw InvalidArgument("font", "null");
    
    offsets.resize(str.length());
    images.resize(str.length());

    bounds = update();
  }
  Text::Text()
  : Locateable(), Rotateable(), Scaleable(), Colorable(), Bounded(), Drawable(),
    str(""), font(nullptr)
  {
  }
  Text::~Text()
  {
  }
  void Text::setString(string str)
  {
    str = str;
    offsets.resize(str.length());
    images.resize(str.length());
    update();
  }
  string Text::getString() const
  {
    return str;
  }
  void Text::draw(const Game& game)
  {
    for(unsigned i = 0; i < str.length(); i++)
    {
      if(images[i].texture != nullptr)
      {
        double height = images[i].texture->getDimensions().y;
        Bounds imageBounds;
        imageBounds.lowerLeft = bounds.lowerLeft + offsets[i];
        imageBounds.upperRight = bounds.upperRight + offsets[i];
        images[i].bounds = imageBounds;
        
        Pair imageLocation = location;
        if(drawCentered)
          imageLocation = imageLocation - center;
        images[i].location = imageLocation;
        
        images[i].rotation = rotation;
        images[i].scale = scale;
        images[i].center = center;
        images[i].draw(game);
      }
    }
  }
  /* populates offsets and images and return the bounds */
  Bounds Text::update()
  {
    double x = 0, min_y = 0, max_y = 0;
    int previous_glyph_index = 0;
    bool do_kerning = FT_HAS_KERNING(font->face);
    for(size_t i = 0; i < str.length(); i++)
    {
      char c = str[i];
      int glyph_index = FT_Get_Char_Index(font->face, c);
      FT_Load_Glyph(font->face, glyph_index, 0);
      if(do_kerning && previous_glyph_index != 0)
      {
        FT_Vector delta;
        FT_Get_Kerning(font->face, previous_glyph_index, glyph_index,
                       FT_KERNING_UNFITTED, &delta);
        x += delta.x;
      }
      offsets[i] = Pair(-x, (double) -(font->face->glyph->metrics.horiBearingY -
                                        font->face->glyph->metrics.height) / 64);
      x += font->face->glyph->advance.x / 64;
      if(-font->face->glyph->metrics.height +
         font->face->glyph->metrics.horiBearingY < min_y)
        min_y = -font->face->glyph->metrics.height +
        font->face->glyph->metrics.horiBearingY;
      if(font->face->glyph->metrics.horiBearingY > max_y)
        max_y = font->face->glyph->metrics.horiBearingY;
    }
    for(size_t i = 0; i < str.length(); i++)
    {
      char c = str[i];
      if(c == ' ')
        images[i].texture = 0;
      else
      {
        images[i] = Image(location, &font->textures[c]);
        images[i].drawCentered = true;
      }
    }
    return Bounds(Pair(0.0, min_y / 64), Pair(x, max_y / 64));
  }
}
