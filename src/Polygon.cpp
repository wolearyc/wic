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
 * File:    Polygon.cpp
 * ----------------------------------------------------------------------------
 */
#include "Polygon.h"
namespace wic
{
  Polygon::Polygon(Pair location_, vector<Pair> vertices, Color color_)
  : Locateable(location_), Rotateable(), Scaleable(), Colorable(color_),
    Drawable()
  {
    setVertices(vertices);
  }
  Polygon::Polygon()
  : Polygon(Pair(), {Pair(), Pair(), Pair()}, Color::White)
  {
  }
  Polygon::Polygon(const Polygon& other)
  : Locateable(other), Rotateable(other), Scaleable(other), Colorable(other),
    Drawable(other), vertices_(other.vertices_)
  {
  }
  void Polygon::setVertices(vector<Pair> vertices)
  {
    if(vertices.size() < 3)
      throw WicError(WIC_ERRNO_SMALL_NUM_VERTICES);
    vertices_ = vertices;
  }
  vector<Pair> Polygon::getVertices() { return vertices_; };
  Pair Polygon::getGeoCenter()
  {
    Pair result = Pair();
    for(unsigned i = 0; i < vertices_.size(); i++)
      result += vertices_[i];
    result /= vertices_.size();
    return result;
  }
  void Polygon::draw(Game& game)
  {
    double cosine = cos(rotation);
    double sine = sin(rotation);
    glColor4ub(color.red, color.green, color.blue,
               color.alpha);
    glBegin(GL_POLYGON);
    for(unsigned i = 0; i < vertices_.size(); i++)
    {
      Pair vertex = vertices_[i];
      vertex = vertex - center;
      vertex = vertex * scale;
      double x = vertex.x * cosine - vertex.y * sine;
      double y = vertex.x * sine + vertex.y * cosine;
      vertex.x = x;
      vertex.y = y;
      if(!drawCentered)
        vertex = vertex + center;
      vertex = convertLocation(vertex+location, game.getDimensions());
      glVertex2d(vertex.x, vertex.y);
    }
    glEnd();
  }
}
