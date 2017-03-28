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
  Text::Text(Pair location_, string str, const Font* font, Color color_)
  : Locateable(location_), Rotateable(), Scaleable(), Colorable(color_),
    Bounded(), Drawable(), str_(str), font_(font)
  {
    if(!font)
      throw InvalidArgument("font", "should not be null");
    
    offsets_.resize(str.length());
    images_.resize(str.length());

    bounds = getData();
  }
  Text::Text()
  : Locateable(), Rotateable(), Scaleable(), Colorable(), Bounded(), Drawable(),
    str_(""), font_(nullptr)
  {
  }
  Text::~Text()
  {
  }
  void Text::setString(string str)
  {
    str_ = str;
    offsets_.resize(str.length());
    images_.resize(str.length());
    bounds = getData();
  }
  string Text::getString() const
  {
    return str_;
  }
  void Text::draw(const Game& game)
  {
    for(unsigned i = 0; i < str_.length(); i++)
    {
      if(images_[i].texture != nullptr)
      {
        double height = images_[i].texture->getDimensions().y;
        Bounds imageBounds;
        imageBounds.lowerLeft = bounds.lowerLeft + offsets_[i];
        imageBounds.upperRight = bounds.upperRight + offsets_[i];
        images_[i].bounds = imageBounds;
        
        Pair imageLocation = location;
        if(drawCentered)
          imageLocation = imageLocation - center;
        images_[i].location = imageLocation;
        
        images_[i].rotation = rotation;
        images_[i].scale = scale;
        images_[i].center = center;
        images_[i].draw(game);
      }
    }
  }
  /* populates offsets and images and return the bounds */
  Bounds Text::getData()
  {
    double x = 0, min_y = 0, max_y = 0;
    int previous_glyph_index = 0;
    bool do_kerning = FT_HAS_KERNING(font_->face_);
    for(size_t i = 0; i < str_.length(); i++)
    {
      char c = str_[i];
      int glyph_index = FT_Get_Char_Index(font_->face_, c);
      FT_Load_Glyph(font_->face_, glyph_index, 0);
      if(do_kerning && previous_glyph_index != 0)
      {
        FT_Vector delta;
        FT_Get_Kerning(font_->face_, previous_glyph_index, glyph_index,
                       FT_KERNING_UNFITTED, &delta);
        x += delta.x;
      }
      offsets_[i] = Pair(-x, (double) -(font_->face_->glyph->metrics.horiBearingY -
                                        font_->face_->glyph->metrics.height) / 64);
      x += font_->face_->glyph->advance.x / 64;
      if(-font_->face_->glyph->metrics.height +
         font_->face_->glyph->metrics.horiBearingY < min_y)
        min_y = -font_->face_->glyph->metrics.height +
        font_->face_->glyph->metrics.horiBearingY;
      if(font_->face_->glyph->metrics.horiBearingY > max_y)
        max_y = font_->face_->glyph->metrics.horiBearingY;
    }
    for(size_t i = 0; i < str_.length(); i++)
    {
      char c = str_[i];
      if(c == ' ')
        images_[i].texture = 0;
      else
      {
        images_[i] = Image(location, &font_->textures_[c]);
        images_[i].drawCentered = true;
      }
    }
    return Bounds(Pair(0.0, min_y / 64), Pair(x, max_y / 64));
  }
}
