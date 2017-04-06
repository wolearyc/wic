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
 * File:    Image.cpp
 * ----------------------------------------------------------------------------
 */
#include "Image.h"
namespace wic
{
  Image::Image(Pair location, const Texture* texture)
  : Locateable(location), Rotateable(), Scaleable(), Colorable(),
    Drawable(), Bounded(Bounds(Pair(), texture->getDimensions())),
    texture(texture)
  {
  }
  Image::Image()
  : Locateable(), Rotateable(), Scaleable(), Colorable(), Drawable(),
    Bounded(), texture(nullptr)
  {
  }
  Image::Image(const Image& other)
  : Locateable(other), Rotateable(other), Scaleable(other), Colorable(other),
    Drawable(other), Bounded(other), texture(other.texture)
  {
  }
  Pair Image::getGeoCenter() const
  {
    Pair diagonal = bounds.upperRight - bounds.lowerLeft;
    return diagonal / 2;
  }
  void Image::draw()
  {
    Pair windowDimensions = getWindowDimensions();
    Pair textureDimensions = texture->getDimensions();
    glBindTexture(GL_TEXTURE_2D, texture->data);
    glColor4ub(color.red, color.green, color.blue,
               color.alpha);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    
    /* lower left corner */
    Pair vertex(0,0);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    glTexCoord2f(bounds.lowerLeft.x / textureDimensions.x,
                 bounds.lowerLeft.y / textureDimensions.y);
    glVertex2d(vertex.x, vertex.y);
    /* lower right corner */
    vertex = Pair(bounds.upperRight.x - bounds.lowerLeft.x, 0.0);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    glTexCoord2f(bounds.upperRight.x / textureDimensions.x,
                 bounds.lowerLeft.y / textureDimensions.y);
    glVertex2d(vertex.x, vertex.y);
    /* upper right corner */
    vertex = bounds.upperRight-bounds.lowerLeft;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    glTexCoord2f(bounds.upperRight.x / textureDimensions.x,
                 bounds.upperRight.y / textureDimensions.y);
    glVertex2d(vertex.x, vertex.y);
    /* upper left corner */
    vertex = Pair(0.0, bounds.upperRight.y - bounds.lowerLeft.y);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    glTexCoord2f(bounds.lowerLeft.x / textureDimensions.x,
                 bounds.upperRight.y / textureDimensions.y);
    glVertex2d(vertex.x, vertex.y);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
  Bounds Image::getWholeBounds()
  {
    return Bounds(Pair(), texture->getDimensions());
  }
}
