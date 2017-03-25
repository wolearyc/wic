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
 * File:    Color.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef COLOR_H
#define COLOR_H
#include <cstdint>
namespace wic
{
  /** \brief an RGB (24 bit) or RGBA (32 bit) color
   *
   *  Color stores 8 bit red, green, blue, and alpha components. For the red,
   *  green, and blue components, a value of 255 indicates complete
   *  expression, while a value of 0 indicates no expression. For the alpha
   *  component, a value of 255 indicates complete opacity while a value of 0
   *  indicates complete transparency.
   *
   *  Color contains some common pre-defined colors, stored as static
   *  variables.
   */
  class Color
  {
  public:
    const static Color White;   /**< white (255,255,255) */
    const static Color Silver;  /**< silver (208,208,208) */
    const static Color Gray;    /**< gray (128,128,128) */
    const static Color Black;   /**< black (0,0,0) */
    const static Color Red;     /**< red (255,0,0) */
    const static Color Maroon;  /**< maroon (128,0,0) */
    const static Color Yellow;  /**< yellow (255,255,0) */
    const static Color Orange;  /**< orange (255,165,0) */
    const static Color Olive;   /**< olive (128,128,0) */
    const static Color Lime;    /**< lime (0,255,0) */
    const static Color Green;   /**< green (0,128,0) */
    const static Color Aqua;    /**< aqua (0,255,255) */
    const static Color Teal;    /**< teal (0,128,128) */
    const static Color Blue;    /**< blue (0,0,255) */
    const static Color Navy;    /**< navy (0,0,128) */
    const static Color Fuchsia; /**< fuschsia (255,0,255) */
    const static Color Purple;  /**< purple (128,0,128) */
    /** \brief constructs  a Color based on red, green, blue, and alpha
     *        components
     *  \param red_ the red component
     *  \param green_ the green component
     *  \param blue_ the blue component
     *  \param alpha_ the alpha component
     */
    Color(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_);
    /** \brief constructs a completely opaque Color based on red, green, and
     *        blue components
     *  \param red_ the red component
     *  \param green_ the green component
     *  \param blue_ the blue component
     */
    Color(uint8_t red_, uint8_t green_, uint8_t blue_);
    /** \brief constructs a Color representing white (255,255,255)
     */
    Color();
    /** \brief constructs a Color identical to another
     *  \param other another Color
     */
    Color(const Color& other);
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
  };
}
#endif
