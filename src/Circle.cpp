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
 * File:    Circle.cpp
 * ----------------------------------------------------------------------------
 */
#include "Circle.h"
namespace wic
{
  Circle::Circle(Pair location, unsigned radius, Color color)
  : Locateable(location), Rotateable(), Scaleable(), Colorable(color),
    Drawable(), radius(radius)
  {
  }
  Circle::Circle()
  : Circle(Pair(), 10, Color::White)
  {
  }
  Circle::Circle(const Circle& other)
  : Locateable(other), Rotateable(other), Scaleable(other), Colorable(other),
    Drawable(other), radius(other.radius)
  {
  }
  Pair Circle::getGeoCenter() const
  {
    return Pair((int) radius, (int) radius);
  }
  void Circle::draw()
  {
    glColor4ub(color.red, color.green, color.blue, color.alpha);
    glBegin(GL_TRIANGLE_FAN);
    // Draw center
    Pair trueCenter = getGeoCenter();
    Pair vertex = trueCenter;
    vertex.transform(rotation, scale, center);
    if(drawCentered)
      vertex -= center;
    vertex = private_wic::getOpenGLVertex(vertex+location);
    const int numTriangles = 50;
    glVertex2f(vertex.x, vertex.y);
    for(int i = 0; i <= numTriangles; i++)
    {
      vertex.x = trueCenter.x + (radius * cos(i * 2 * M_PI / numTriangles));
      vertex.y = trueCenter.x + (radius * sin(i * 2 * M_PI / numTriangles));
      vertex.transform(rotation, scale, center);
      if(drawCentered)
        vertex -= center;
      vertex = private_wic::getOpenGLVertex(vertex+location);
      glVertex2f(vertex.x, vertex.y);
    }
    glEnd();
  }
}

