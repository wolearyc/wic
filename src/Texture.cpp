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
  Texture::Texture(vector<uint8_t> buffer, Pair dimensions, enum Format format,
                   enum Filter filter, enum Wrap wrap)
  : Texture()
  {
    init(buffer, dimensions, format, filter, wrap);
  }
  Texture::Texture(string filepath, enum Filter filter, enum Wrap wrap)
  : Texture()
  {
    unsigned char* buffer = nullptr;
    int x = 0;
    int y = 0;
    buffer = SOIL_load_image(filepath.data(), &x, &y, 0, SOIL_LOAD_RGBA);
    dimensions = Pair(x,y);
    if(buffer == nullptr)
      throw InvalidFile(filepath);
    int size = x * y * 4;
    init(vector<uint8_t>(buffer, buffer+size), dimensions,
         Format::RGBA, filter, wrap);
    SOIL_free_image_data(buffer);
  }
  Texture::Texture(string filepath)
  : Texture(filepath, Filter::Nearest, Wrap::Repeat)
  {
  }
  Texture::Texture(const Texture& other)
  : data(other.data), dimensions(other.dimensions), loaded(other.loaded),
    formattedBuffer(other.formattedBuffer), filter(other.filter),
    wrap(other.wrap)
  {
  }
  Texture::Texture()
  : data(0), dimensions(Pair()), loaded(false), filter(Filter::Nearest),
    wrap(Wrap::Repeat)
  {
  }
  Texture::~Texture()
  {
    if(loaded)
      glDeleteTextures(1, &data);
  }
  void Texture::load()
  {
    private_wic::submitTexture(&data, (int) wrap, (int) filter,
                               dimensions, formattedBuffer);
    loaded = true;
  }
  Pair Texture::getDimensions() const
  {
    return dimensions;
  }
  void Texture::init(vector<uint8_t> buffer, Pair dimensions, enum Format format,
                     enum Filter filter, enum Wrap wrap)
  {
    if(dimensions.x < 1)
      throw InvalidArgument("dimensions.x", "zero");
    if(dimensions.y < 1)
      throw InvalidArgument("dimensions.y", "zero");
    if((format == Format::Mono || format == Format::Grayscale) &&
       (int) (dimensions.x * dimensions.y) != buffer.size())
      throw InvalidArgument("dimensions", "incompatible with buffer");
    else if(format == Format::RGB &&
       (int) (dimensions.x * dimensions.y * 3) != buffer.size())
      throw InvalidArgument("dimensions", "incompatible with buffer");
    else if(format == Format::RGBA &&
            (int) (dimensions.x * dimensions.y * 4) != buffer.size())
      throw InvalidArgument("dimensions", "incompatible with buffer");
    
    this->dimensions = dimensions;
    this->filter = filter;
    this->wrap = wrap;
    
    int xDimension = (int) (dimensions.x) * 4;
    int yDimension = (int) dimensions.y;
    vector<vector<uint8_t>> temp(xDimension, vector<uint8_t>(yDimension));
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
    formattedBuffer.resize(xDimension * yDimension);
    
    int formattedBufferIndex = 0;
    for(int y = yDimension-1; y >= 0; y--) /* flips texture */
    {
      for(int x = 0; x < xDimension; x++)
      {
        formattedBuffer[formattedBufferIndex] = temp[x][y];
        formattedBufferIndex++;
      }
    }
    // Texture binding and buffer deallocation done at next updt.
  }
}
