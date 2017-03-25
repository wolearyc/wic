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
 * File:    Image.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "Game.h"
namespace wic
{
  /** \brief an object that can be drawn to the screen */
  class Drawable
  {
  public:
    /** \brief Constructor
     *  
     *  \param drawCentered_ whether or not to draw around some center
     */
    Drawable(bool drawCentered_);
    /** \brief Default constructor */
    Drawable();
    /** \brief Copy constructor */
    Drawable(const Drawable& other);
    /** \brief draws to the screen 
     *  \param game the game
     */
    virtual void draw(Game& game) = 0;
    bool drawCentered; /**< whether or not to draw around some center */
  };
}
#endif
