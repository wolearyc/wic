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
 * File:    Color.cpp
 * ----------------------------------------------------------------------------
 */
#include "Color.h"
namespace wic
{
  const Color Color::White   = Color(255,255,255);
  const Color Color::Silver  = Color(208,208,208);
  const Color Color::Gray    = Color(128,128,128);
  const Color Color::Black   = Color(0,0,0);
  const Color Color::Red     = Color(255,0,0);
  const Color Color::Maroon  = Color(128,0,0);
  const Color Color::Yellow  = Color(255,255,0);
  const Color Color::Orange  = Color(255,165,0);
  const Color Color::Olive   = Color(128,128,0);
  const Color Color::Lime    = Color(0,255,0);
  const Color Color::Green   = Color(0,128,0);
  const Color Color::Aqua    = Color(0,255,255);
  const Color Color::Teal    = Color(0,128,128);
  const Color Color::Blue    = Color(0,0,255);
  const Color Color::Navy    = Color(0,0,128);
  const Color Color::Fuchsia = Color(255,0,255);
  const Color Color::Purple  = Color(128,0,128);
  Color::Color(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_)
  :red(red_), green(green_), blue(blue_), alpha(alpha_)
  {
  }
  Color::Color(uint8_t red_, uint8_t green_, uint8_t blue_)
  :Color(red_, green_, blue_, 255)
  {
  }
  Color::Color(const Color &other)
  :red(other.red), green(other.green), blue(other.blue),
  alpha(other.alpha)
  {
  }
  Color::Color()
  :Color(255,255,255)
  {
  }
}
