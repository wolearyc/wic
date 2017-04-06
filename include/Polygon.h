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
 * File:    Polygon.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include "Interfaces.h"
using std::vector;
namespace wic
{
  /** A filled polygon. */
  class Polygon
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Drawable
  {
  public:
    /** Constructor.
     *  \param location the screen location
     *  \param vertices  the vertices in drawing order; must contain >2 vertices
     *  \param color the desired color
     */
    Polygon(Pair location, vector<Pair> vertices, Color color);
    /** Default constructor. */
    Polygon();
    /** Copy constructor. */
    Polygon(const Polygon& other);
    void draw();
    /** Returns the geometric center. */
    Pair getGeoCenter() const;
    /** Sets new vertices.
     *  \param vertices the new vertices in drawing order; must contain >2 
     *         vertices
     */
    void setVertices(vector<Pair> vertices);
    /** Returns the list of vertices in drawing order. */
    vector<Pair> getVertices() const;
  protected:
    vector<Pair> vertices;
  };
}
#endif
