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
  /** \brief a filled polygon */
  class Polygon
  : public Locateable, public Rotateable, public Scaleable, public Colorable,
    public Drawable
  {
  public:
    /** \brief Constructor
     *  \param location_ the desired screen location
     *  \param vertices_ the vertices in drawing order; must contain >2 vertices
     *  \param color_ the desired color
     */
    Polygon(Pair location_, vector<Pair> vertices, Color color_);
    /** \brief Default constructor */
    Polygon();
    /** \brief Copy constructor */
    Polygon(const Polygon& other);
    void draw(const Game& game);
    /** \brief Gets the geometric center */
    Pair getGeoCenter() const;
    /** \brief sets new vertices
     *  \param vertices the new vertices in drawing order; must contain >2 
     *         vertices
     */
    void setVertices(vector<Pair> vertices);
    /** \brief gets the list of vertices in drawing order */
    vector<Pair> getVertices() const;
  protected:
    vector<Pair> vertices_;
  };
}
#endif
