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
 * File:    Quad.cpp
 * ----------------------------------------------------------------------------
 */
#include "Quad.h"
namespace wic
{
  static double vertices[8] = {0}; /* utility buffer for vertices */
  Quad::Quad(Pair location, Pair dimensions, Color color)
  : Locateable(location), Rotateable(), Scaleable(), Colorable(color),
    Drawable(), dimensions(dimensions)
  {
  }
  Quad::Quad()
  : Quad(Pair(), Pair(10,10), Color::White)
  {
  }
  Quad::Quad(const Quad& other)
  : Locateable(other), Rotateable(other), Scaleable(other), Colorable(other),
    Drawable(other), dimensions(other.dimensions)
  {
  }
  Pair Quad::getGeoCenter() const
  {
    return dimensions / 2;
  }
  void Quad::draw()
  {
    glColor4ub(color.red, color.green, color.blue, color.alpha);
    
    Pair vertex;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[0] = vertex.x;
    vertices[1] = vertex.y;
    
    vertex = Pair(dimensions.x, 0.0);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[2] = vertex.x;
    vertices[3] = vertex.y;
    
    vertex = Pair(0.0, dimensions.y);
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[4] = vertex.x;
    vertices[5] = vertex.y;
    
    vertex = dimensions;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    vertices[6] = vertex.x;
    vertices[7] = vertex.y;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_DOUBLE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

