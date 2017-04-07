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
  // Utility buffers.
  static double vertices[8] = {0};
  static double textureVertices[8] = {0};

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
    Pair textureDimensions = texture->getDimensions();
    // Lower left corner.
    Pair vertex;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[0] = vertex.x;
    vertices[1] = vertex.y;
    textureVertices[0] = bounds.lowerLeft.x / textureDimensions.x;
    textureVertices[1] = bounds.lowerLeft.y / textureDimensions.y;
    // Lower right corner.
    vertex = Pair(bounds.upperRight.x - bounds.lowerLeft.x, 0.0);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[2] = vertex.x;
    vertices[3] = vertex.y;
    textureVertices[2] = bounds.upperRight.x / textureDimensions.x;
    textureVertices[3] = bounds.lowerLeft.y / textureDimensions.y;
    // Upper left corner/
    vertex = Pair(0.0, bounds.upperRight.y - bounds.lowerLeft.y);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[4] = vertex.x;
    vertices[5] = vertex.y;
    textureVertices[4] = bounds.lowerLeft.x / textureDimensions.x;
    textureVertices[5] = bounds.upperRight.y / textureDimensions.y;
    // Upper right corner.
    vertex = bounds.upperRight-bounds.lowerLeft;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[6] = vertex.x;
    vertices[7] = vertex.y;
    textureVertices[6] = bounds.upperRight.x / textureDimensions.x;
    textureVertices[7] = bounds.upperRight.y / textureDimensions.y;
    
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_DOUBLE, 0, textureVertices);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glBindTexture(GL_TEXTURE_2D, texture->data);
    glColor4ub(color.red, color.green, color.blue, color.alpha);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisable(GL_TEXTURE_2D);
  }
  Bounds Image::getWholeBounds() const
  {
    return Bounds(Pair(), texture->getDimensions());
  }
}
