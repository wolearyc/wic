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
 * File:    Color.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef COLOR_H
#define COLOR_H
#include <cstdint>
namespace wic
{
  /** A color. Color stores 8 bit red, green, blue, and alpha components. 
   *  For the red, green, and blue components, a value of 255 indicates complete
   *  expression, while a value of 0 indicates no expression. For the alpha
   *  component, a value of 255 indicates complete opacity while a value of 0
   *  indicates complete transparency. Color also stores predefined colors which
   *  are accessible as static variables.
   */
  class Color
  {
  public:
    const static Color White;     /**< White (255,255,255). */
    const static Color Silver;    /**< Silver (208,208,208). */
    const static Color Gray;      /**< Gray (128,128,128). */
    const static Color Black;     /**< Black (0,0,0). */
    const static Color Red;       /**< Red (255,0,0). */
    const static Color Maroon;    /**< Maroon (128,0,0). */
    const static Color Yellow;    /**< Yellow (255,255,0). */
    const static Color Orange;    /**< Orange (255,165,0). */
    const static Color Olive;     /**< Olive (128,128,0). */
    const static Color Lime;      /**< Lime (0,255,0). */
    const static Color Green;     /**< Green (0,128,0). */
    const static Color Aqua;      /**< Aqua (0,255,255). */
    const static Color Teal;      /**< Teal (0,128,128). */
    const static Color Blue;      /**< Blue (0,0,255). */
    const static Color Navy;      /**< Navy (0,0,128). */
    const static Color Fuchsia;   /**< Fuschsia (255,0,255). */
    const static Color Purple;    /**< Purple (128,0,128). */
    const static Color WicOrange; /**< Wic's standard orange. */
    const static Color WicGray;   /**< Wic's standard gray. */
    /** Constructor.
     *  \param red the red component
     *  \param green the green component
     *  \param blue the blue component
     *  \param alpha the alpha component
     */
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    /** Constructor (opaque).
     *  \param red the red component
     *  \param green the green component
     *  \param blue the blue component
     */
    Color(uint8_t red, uint8_t green, uint8_t blue);
    /** Default constructor (white). */
    Color();
    /** Copy constructor. */
    Color(const Color& other);
    uint8_t red;    /**< The red component */
    uint8_t green;  /**< The green component */
    uint8_t blue;   /**< The blue component */
    uint8_t alpha;  /**< The alpha component */
  };
}
#endif
