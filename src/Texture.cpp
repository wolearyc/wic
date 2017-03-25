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
 * File:    Texture.cpp
 * ----------------------------------------------------------------------------
 */
#include "Texture.h"
namespace wic
{
  Texture::Texture(unsigned char* buffer, Pair dimensions, enum WicFormat format,
                   enum WicFilter filter, enum WicWrap wrap)
  {
    if(!buffer)
      throw WicError(WIC_ERRNO_NULL_BUFFER);
    if(dimensions.x < 1)
      throw WicError(WIC_ERRNO_SMALL_X_DIMENSION);
    if(dimensions.y < 1)
      throw WicError(WIC_ERRNO_SMALL_Y_DIMENSION);
    int x_dimension = (int) (dimensions.x) * 4;
    int y_dimension = (int) dimensions.y;
    unsigned char** temp = (unsigned char**) malloc(x_dimension * sizeof(unsigned char*));
    if(!temp)
      throw WicError(WIC_ERRNO_NO_HEAP);
    for(int x = 0; x < x_dimension; x++)
    {
      temp[x] = (unsigned char*) malloc(y_dimension * sizeof(unsigned char));
      if(!temp[x])
      {
        for(int i = x-1; i < 0; i--)
          free(temp[i]);
        free(temp);
        throw WicError(WIC_ERRNO_NO_HEAP);
      }
    }
    for(int y = 0; y < y_dimension; y++)
    {
      for(int x = 0; x < x_dimension; x+=4)
      {
        temp[x][y] = 255;
        temp[x+1][y] = 255;
        temp[x+2][y] = 255;
        temp[x+3][y] = 0;
      }
    }
    int buffer_index = 0;
    for(int y = 0; y < y_dimension; y++)
    {
      for(int x = 0; x < x_dimension; x+=4)
      {
        if(format == WIC_MONO)
        {
          unsigned char character = buffer[buffer_index];
          if(!character)
            temp[x+3][y] = 0;
          else
            temp[x+3][y] = 255;
          buffer_index++;
        }
        else if(format == WIC_GREYSCALE)
        {
          temp[x+3][y] = buffer[buffer_index];
          buffer_index++;
        }
        else if(format == WIC_RGB)
        {
          temp[x][y] = buffer[buffer_index];
          temp[x+1][y] = buffer[buffer_index+1];
          temp[x+2][y] = buffer[buffer_index+2];
          temp[x+3][y] = 255;
          buffer_index += 3;
        }
        else if(format == WIC_RGBA)
        {
          temp[x][y] = buffer[buffer_index];
          temp[x+1][y] = buffer[buffer_index+1];
          temp[x+2][y] = buffer[buffer_index+2];
          temp[x+3][y] = buffer[buffer_index+3];
          buffer_index += 4;
        }
      }
    }
    unsigned char* formatted_buffer = (unsigned char*) malloc(x_dimension * y_dimension *
                                                              sizeof(unsigned char));
    if(!formatted_buffer)
    {
      for(int x = 0; x < x_dimension; x++)
        free(temp[x]);
      free(temp);
      throw WicError(WIC_ERRNO_NO_HEAP);
    }
    int formatted_buffer_index = 0;
    for(int y = y_dimension-1; y >= 0; y--) /* flips texture */
    {
      for(int x = 0; x < x_dimension; x++)
      {
        formatted_buffer[formatted_buffer_index] = temp[x][y];
        formatted_buffer_index++;
      }
    }
    for(int x = 0; x < x_dimension; x++)
      free(temp[x]);
    free(temp);
    unsigned int data;
    glGenTextures(1, &data);
    glBindTexture(GL_TEXTURE_2D, data);
    if(wrap == WIC_STOP)
    {
      float color[] = { 1.0f, 1.0f, 1.0f, 0.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, formatted_buffer);
    free(formatted_buffer);
    if(glGetError() == GL_OUT_OF_MEMORY)
    {
      glDeleteTextures(1, &data);
      throw WicError(WIC_ERRNO_NO_GPU_MEM);
    }
    
    data_ = data;
    dimensions_ = dimensions;
  }
  Texture::Texture(string filepath, enum WicFilter filter, enum WicWrap wrap)
  {
    unsigned char* buffer = 0;
    int x = 0;
    int y = 0;
    buffer = SOIL_load_image(filepath.data(), &x, &y, 0, SOIL_LOAD_RGBA);
    Pair dimensions = (Pair) {x,y};
    if(!buffer)
      throw WicError(WIC_ERRNO_LOAD_FILE_FAIL);
    Texture(buffer, dimensions, WIC_RGBA, filter, wrap);
    SOIL_free_image_data(buffer);
  }
  Texture::Texture()
  : data_(0), dimensions_(Pair())
  {
  }
  Texture::~Texture()
  {
    glDeleteTextures(1, &data_);
  }
  Pair Texture::getDimensions()
  {
    return dimensions_;
  }
}
