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
 * File:    Color.cpp
 * ----------------------------------------------------------------------------
 */
#include "Color.h"
namespace wic
{
  const Color Color::White     = Color(255,255,255);
  const Color Color::Silver    = Color(208,208,208);
  const Color Color::Gray      = Color(128,128,128);
  const Color Color::Black     = Color(0,0,0);
  const Color Color::Red       = Color(255,0,0);
  const Color Color::Maroon    = Color(128,0,0);
  const Color Color::Yellow    = Color(255,255,0);
  const Color Color::Orange    = Color(255,165,0);
  const Color Color::Olive     = Color(128,128,0);
  const Color Color::Lime      = Color(0,255,0);
  const Color Color::Green     = Color(0,128,0);
  const Color Color::Aqua      = Color(0,255,255);
  const Color Color::Teal      = Color(0,128,128);
  const Color Color::Blue      = Color(0,0,255);
  const Color Color::Navy      = Color(0,0,128);
  const Color Color::Fuchsia   = Color(255,0,255);
  const Color Color::Purple    = Color(128,0,128);
  const Color Color::WicOrange = Color(253,144,11,255);
  const Color Color::WicGray   = Color(63,63,63,255);
  Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
  : red(red), green(green), blue(blue), alpha(alpha)
  {
  }
  Color::Color(uint8_t red, uint8_t green, uint8_t blue)
  : Color(red, green, blue, 255)
  {
  }
  Color::Color(const Color &other) 
  : red(other.red), green(other.green), blue(other.blue), alpha(other.alpha)
  {
  }
  Color::Color() 
  : Color(255,255,255)
  {
  }
  void Color::lighten(uint8_t factor)
  {
    if(factor > 255 - red)
      red = 255;
    else
      red += factor;
    if(factor > 255 - green)
      green = 255;
    else
      green += factor;
    if(factor > 255 - blue)
      blue = 255;
    else
      blue += factor;
  }
  void Color::darken(uint8_t factor)
  {
    if(factor > red)
      red = 0;
    else
      red -= factor;
    if(factor > green)
      green = 0;
    else
      green -= factor;
    if(factor > blue)
      blue = 0;
    else
      blue -= factor;
  }
}
