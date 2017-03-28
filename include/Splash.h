/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for MacOS written in C++
 * Copyright (C) 2013-2017  Willis O'Leary
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.s
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 * File:    Splash.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef SPLASH_H
#define SPLASH_H
#include <math.h>
#include "Image.h"
#include "Error.h"
#include "Color.h"
#include "Game.h"
#include "Quad.h"
namespace wic
{
  /** \brief draws the wic splash screen and exits when the animation completes
   *  \param backgroundColor the desired background color (try Color::WicGray)
   *  \param textColor the desired text color (try Color::WicOrange)
   *  \param game the game
   */
  void drawSplash(Color backgroundColor, Color textColor, const Game& game);
}
#endif
