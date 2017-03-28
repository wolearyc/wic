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
 * File:    Texture.cpp
 * ----------------------------------------------------------------------------
 */
#include "Texture.h"
namespace wic
{
  Texture::Texture(unsigned char* buffer, Pair dimensions, enum Format format,
                   enum Filter filter, enum Wrap wrap)
  {
    if(!buffer)
      throw InvalidArgument("buffer", "should not be null");
    if(dimensions.x < 1)
      throw InvalidArgument("dimensions.x", "should be nonzero");
    if(dimensions.y < 1)
      throw InvalidArgument("dimensions.y", "should be nonzero");
    int x_dimension = (int) (dimensions.x) * 4;
    int y_dimension = (int) dimensions.y;
    unsigned char** temp = new unsigned char*[x_dimension];
    for(int x = 0; x < x_dimension; x++)
    {
      temp[x] = new unsigned char[y_dimension];
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
    int bufferIndex = 0;
    for(int y = 0; y < y_dimension; y++)
    {
      for(int x = 0; x < x_dimension; x+=4)
      {
        if(format == Format::Mono)
        {
          unsigned char character = buffer[bufferIndex];
          if(!character)
            temp[x+3][y] = 0;
          else
            temp[x+3][y] = 255;
          bufferIndex++;
        }
        else if(format == Format::Grayscale)
        {
          temp[x+3][y] = buffer[bufferIndex];
          bufferIndex++;
        }
        else if(format == Format::RGB)
        {
          temp[x][y] = buffer[bufferIndex];
          temp[x+1][y] = buffer[bufferIndex+1];
          temp[x+2][y] = buffer[bufferIndex+2];
          temp[x+3][y] = 255;
          bufferIndex += 3;
        }
        else if(format == Format::RGBA)
        {
          temp[x][y] = buffer[bufferIndex];
          temp[x+1][y] = buffer[bufferIndex+1];
          temp[x+2][y] = buffer[bufferIndex+2];
          temp[x+3][y] = buffer[bufferIndex+3];
          bufferIndex += 4;
        }
      }
    }
    unsigned char* formattedBuffer = new unsigned char[x_dimension*y_dimension];

    int formattedBufferIndex = 0;
    for(int y = y_dimension-1; y >= 0; y--) /* flips texture */
    {
      for(int x = 0; x < x_dimension; x++)
      {
        formattedBuffer[formattedBufferIndex] = temp[x][y];
        formattedBufferIndex++;
      }
    }
    for(int x = 0; x < x_dimension; x++)
      free(temp[x]);
    free(temp);
    unsigned int data;
    glGenTextures(1, &data);
    glBindTexture(GL_TEXTURE_2D, data);
    if(wrap == Wrap::Stop)
    {
      float color[] = { 1.0f, 1.0f, 1.0f, 0.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dimensions.x, dimensions.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedBuffer);
    free(formattedBuffer);
    if(glGetError() == GL_OUT_OF_MEMORY)
    {
      glDeleteTextures(1, &data);
      throw Error("out of GPU memory");
    }
    
    data_ = data;
    dimensions_ = dimensions;
  }
  Texture::Texture(string filepath, enum Filter filter, enum Wrap wrap)
  {
    unsigned char* buffer = 0;
    int x = 0;
    int y = 0;
    buffer = SOIL_load_image(filepath.data(), &x, &y, 0, SOIL_LOAD_RGBA);
    Pair dimensions(x,y);
    if(!buffer)
      throw InvalidFile(filepath);
    Texture(buffer, dimensions, Format::RGBA, filter, wrap);
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
  Pair Texture::getDimensions() const
  {
    return dimensions_;
  }
}
