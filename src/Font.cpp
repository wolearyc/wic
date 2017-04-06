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
 * File:    Font.cpp
 * ----------------------------------------------------------------------------
 */
#include "Font.h"
namespace wic
{
  Font::Font(string filepath, unsigned point, bool antialias)
  : point(point), antialias(antialias)
  {
    if(point == 0)
      throw InvalidArgument("point", "zero");
    int error = FT_New_Face(private_wic::getFTLibrary(), filepath.data(), 0,
                            &face);
    if(error != 0)
      throw InvalidFile(filepath);
    
    Pair pixelDensity = getPixelDensity();
    FT_Set_Char_Size(face, 0, point*64, pixelDensity.x, pixelDensity.y);
    for(unsigned char c = 0; c < NUM_CHARS; c++)
    {
      int glyph_index = FT_Get_Char_Index(face, c);
      if(antialias)
        FT_Load_Glyph(face, glyph_index, FT_LOAD_FORCE_AUTOHINT);
      else
        FT_Load_Glyph(face, glyph_index, 0);
      if(antialias)
      {
        if(!FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) &&
           face->glyph->bitmap.buffer != 0)
        {
          Pair dimensions(face->glyph->bitmap.width, face->glyph->bitmap.rows);
          int bufferSize = (int) (dimensions.x + dimensions.y);
          vector<uint8_t> buffer(face->glyph->bitmap.buffer,
                                 face->glyph->bitmap.buffer + bufferSize);
          textures[c] = Texture(buffer, dimensions, Format::Grayscale,
                                Filter::Nearest, Wrap::Stop);
        }
      }
      else
      {
        if(!FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO) &&
           face->glyph->bitmap.buffer != 0)
        {
          FT_Bitmap target;
          FT_Bitmap_New(&target);
          int error = FT_Bitmap_Convert(private_wic::getFTLibrary(),
                                        &face->glyph->bitmap,
                                        &target, 1);
          Pair dimensions(target.width, target.rows);
          int bufferSize = (int) (dimensions.x + dimensions.y);
          vector<uint8_t> buffer(target.buffer, target.buffer + bufferSize);
          textures[c] = Texture(buffer, dimensions, Format::Mono,
                                Filter::Nearest, Wrap::Stop);
          FT_Bitmap_Done(private_wic::getFTLibrary(), &target);
        }
      }
    }
  }
  Font::Font()
  : face(0), point(0), antialias(false)
  {
  }
  Font::~Font()
  {
    FT_Done_Face(face);
  }
}
