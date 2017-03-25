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
 * File:    Font.cpp
 * ----------------------------------------------------------------------------
 */
#include "Font.h"
namespace wic
{
  static const unsigned char WIC_FONT_NUM_CHARS = 128;
  
  Font::Font(string filepath, unsigned point, bool antialias, Game& game)
  {
    if(!point)
      throw WicError(WIC_ERRNO_SMALL_POINT);
    FT_Face face;
    int error = FT_New_Face(game.FTLibrary_, filepath.data(), 0, &face);
    if(error != 0)
      throw WicError(WIC_ERRNO_LOAD_FILE_FAIL);
    Texture** textures = (Texture**) malloc(WIC_FONT_NUM_CHARS * sizeof(Texture*));
    bzero(textures, WIC_FONT_NUM_CHARS * sizeof(Texture*));
    if(!textures)
    {
      FT_Done_Face(face);
      throw WicError(WIC_ERRNO_NO_HEAP);
    }
    FT_Set_Char_Size(face, 0, point*64, game.pixelDensity_.x,
                     game.pixelDensity_.y);
    for(unsigned char c = 0; c < WIC_FONT_NUM_CHARS; c++)
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
          Pair dimensions = {face->glyph->bitmap.width,
            face->glyph->bitmap.rows};
          textures[c] = new Texture(face->glyph->bitmap.buffer, dimensions,
                                    WIC_GREYSCALE, WIC_NEAREST, WIC_STOP);
          if(!textures[c])
          {
            c--;
            for(; c >= 0; c--)
              delete textures[c];
            free(textures);
            throw WicError(WIC_ERRNO_NO_HEAP);
          }
        }
      }
      else
      {
        if(!FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO) &&
           face->glyph->bitmap.buffer != 0)
        {
          FT_Bitmap target;
          FT_Bitmap_New(&target);
          int error = FT_Bitmap_Convert(game.FTLibrary_,
                                        &face->glyph->bitmap,
                                        &target, 1);
          Pair dimensions = {target.width, target.rows};
          textures[c] = new Texture(target.buffer, dimensions, WIC_MONO,
                                    WIC_NEAREST,WIC_STOP);
          FT_Bitmap_Done(game.FTLibrary_, &target);
          if(!textures[c])
          {
            c--;
            for(; c >= 0; c--)
              delete textures[c];
            FT_Done_Face(face);
            throw WicError(WIC_ERRNO_NO_HEAP);
          }
        }
      }
    }

    face_ = face;
    textures_ = textures;
    point_ = point;
    antialias_ = antialias;
  }
  Font::Font()
  : face_(0), textures_(0), point_(0), antialias_(false)
  {
  }
  Font::~Font()
  {
    FT_Done_Face(face_);
    for(unsigned char c = 0; c < WIC_FONT_NUM_CHARS; c++)
    {
      if(textures_[c] != 0)
      {
        delete textures_[c];
      }
    }
    free(textures_);
  }
}
