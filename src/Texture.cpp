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
  : dimensions(dimensions)
  {
    if(buffer == nullptr)
      throw InvalidArgument("buffer", "should not be null");
    if(dimensions.x < 1)
      throw InvalidArgument("dimensions.x", "zero");
    if(dimensions.y < 1)
      throw InvalidArgument("dimensions.y", "zero");
    
    int xDimension = (int) (dimensions.x) * 4;
    int yDimension = (int) dimensions.y;
    unsigned char** temp = new unsigned char*[xDimension];
    for(int x = 0; x < xDimension; x++)
      temp[x] = new unsigned char[yDimension];
    for(int y = 0; y < yDimension; y++)
    {
      for(int x = 0; x < xDimension; x+=4)
      {
        temp[x][y] = 255;
        temp[x+1][y] = 255;
        temp[x+2][y] = 255;
        temp[x+3][y] = 0;
      }
    }
    int bufferIndex = 0;
    for(int y = 0; y < yDimension; y++)
    {
      for(int x = 0; x < xDimension; x+=4)
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
    unsigned char* formattedBuffer = new unsigned char[xDimension*yDimension];

    int formattedBufferIndex = 0;
    for(int y = yDimension-1; y >= 0; y--) /* flips texture */
    {
      for(int x = 0; x < xDimension; x++)
      {
        formattedBuffer[formattedBufferIndex] = temp[x][y];
        formattedBufferIndex++;
      }
    }
    for(int x = 0; x < xDimension; x++)
      delete[] temp[x];
    delete[] temp;
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
    delete[] formattedBuffer;
    if(glGetError() == GL_OUT_OF_MEMORY)
    {
      glDeleteTextures(1, &data);
      throw Error("out of GPU memory");
    }
  }
  Texture::Texture(string filepath, enum Filter filter, enum Wrap wrap)
  {
    unsigned char* buffer = nullptr;
    int x = 0;
    int y = 0;
    buffer = SOIL_load_image(filepath.data(), &x, &y, 0, SOIL_LOAD_RGBA);
    Pair dimensions(x,y);
    if(buffer == nullptr)
      throw InvalidFile(filepath);
    Texture(buffer, dimensions, Format::RGBA, filter, wrap);
    SOIL_free_image_data(buffer);
  }
  Texture::Texture()
  : data(0), dimensions(Pair())
  {
  }
  Texture::~Texture()
  {
    glDeleteTextures(1, &data);
  }
  Pair Texture::getDimensions() const
  {
    return dimensions;
  }
}
