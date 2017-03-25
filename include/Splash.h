/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for Mac OSX written in C++
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
  extern const Color SPLASH_BACKGROUND_COLOR; /**< standard and preferred
                                                   *   splash screen background
                                                   *   color
                                                   */
  extern const Color SPLASH_TEXT_COLOR; /**< standard and preferred splash
                                             *   screen text color
                                             */
  /** \brief draws the wic splash screen and exits when the animation completes
   *
   *  A simple call to wic_draw_splash before entering your game code is not
   *  required, but the publicity and credit is greatly appreciated. As a bonus,
   *  the wic splash screen makes your game more polished and professional.
   *  \param backgroundColor the desired background color (use of
   *         WIC_SPLASH_BACKGROUND_COLOR is encouraged)
   *  \param textColor the desired text color (use of WIC_SPLASH_TEXT_COLOR is
   *         encouraged)
   *  \param game the game
   *  \return true on success, false on failure
   */
  void drawSplash(Color backgroundColor, Color textColor, Game&
                  game);
}
#endif
