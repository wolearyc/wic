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
 * File:    Polygon.cpp
 * ----------------------------------------------------------------------------
 */
#include "Polygon.h"
namespace wic
{
  Polygon::Polygon(Pair location, vector<Pair> vertices, Color color)
  : Locateable(location), Rotateable(), Scaleable(), Colorable(color),
    Drawable()
  {
    setVertices(vertices);
  }
  Polygon::Polygon()
  : Polygon(Pair(), {Pair(), Pair(50,50), Pair(50,0)}, Color::White)
  {
  }
  Polygon::Polygon(const Polygon& other)
  : Locateable(other), Rotateable(other), Scaleable(other), Colorable(other),
    Drawable(other), vertices(other.vertices)
  {
  }
  void Polygon::setVertices(vector<Pair> vertices)
  {
    if(vertices.size() < 3)
      throw InvalidArgument("vertices length", "< 3");
    this->vertices = vertices;
  }
  vector<Pair> Polygon::getVertices() const { return vertices; };
  Pair Polygon::getGeoCenter() const
  {
    Pair result = Pair();
    for(unsigned i = 0; i < vertices.size(); i++)
      result += vertices[i];
    result /= vertices.size();
    return result;
  }
  void Polygon::draw(const Game& game)
  {
    double cosine = cos(rotation);
    double sine = sin(rotation);
    glColor4ub(color.red, color.green, color.blue, color.alpha);
    glBegin(GL_POLYGON);
    for(unsigned i = 0; i < vertices.size(); i++)
    {
      Pair vertex = vertices[i];
      vertex.transform(rotation, scale, center);
      if(drawCentered)
        vertex -= center;
      vertex = convertLocation(vertex+location, game.getDimensions());
      glVertex2d(vertex.x, vertex.y);
    }
    glEnd();
  }
}
