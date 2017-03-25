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
 * File:    Locateable.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef LOCATEABLE_H
#define LOCATEABLE_H
#include "Pair.h"
namespace wic
{
  /** \brief An object possessing a location
   *
   *  Most objects are 2D, necessitating the specification of a center about
   *  which the location is defined.
   */
  class Locateable
  {
  public:
    /** \brief Constructor
     *
     *  \param location_ the location
     *  \param center_ the center
     */
    Locateable(Pair location_, Pair center_);
    /** \brief Constructor
     *
     *  \param location_ the location
     */
    Locateable(Pair location_);
    /** \brief Default constructor */
    Locateable();
    /** \brief Copy constructor */
    Locateable(const Locateable& other);
    Pair location;    /**< the location */
    Pair center;      /**< the center */
  };
}
#endif
