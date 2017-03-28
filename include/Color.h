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
  /** \brief an RGB (24 bit) or RGBA (32 bit) color
   *
   *  Color stores 8 bit red, green, blue, and alpha components. For the red,
   *  green, and blue components, a value of 255 indicates complete
   *  expression, while a value of 0 indicates no expression. For the alpha
   *  component, a value of 255 indicates complete opacity while a value of 0
   *  indicates complete transparency.
   *
   *  Color contains some common pre-defined colors that can be accessed
   *  as static variables.
   */
  class Color
  {
  public:
    const static Color White;     /**< \brief white (255,255,255) */
    const static Color Silver;    /**< \brief silver (208,208,208) */
    const static Color Gray;      /**< \brief gray (128,128,128) */
    const static Color Black;     /**< \brief black (0,0,0) */
    const static Color Red;       /**< \brief red (255,0,0) */
    const static Color Maroon;    /**< \brief maroon (128,0,0) */
    const static Color Yellow;    /**< \brief yellow (255,255,0) */
    const static Color Orange;    /**< \brief orange (255,165,0) */
    const static Color Olive;     /**< \brief olive (128,128,0) */
    const static Color Lime;      /**< \brief lime (0,255,0) */
    const static Color Green;     /**< \brief green (0,128,0) */
    const static Color Aqua;      /**< \brief aqua (0,255,255) */
    const static Color Teal;      /**< \brief teal (0,128,128) */
    const static Color Blue;      /**< \brief blue (0,0,255) */
    const static Color Navy;      /**< \brief navy (0,0,128) */
    const static Color Fuchsia;   /**< \brief fuschsia (255,0,255) */
    const static Color Purple;    /**< \brief purple (128,0,128) */
    const static Color WicOrange; /**< \brief Wic's standard orange */
    const static Color WicGray;   /**< \brief Wic's standard gray */
    /** \brief Constructor
     *  \param red_ the red component
     *  \param green_ the green component
     *  \param blue_ the blue component
     *  \param alpha_ the alpha component
     */
    Color(uint8_t red_, uint8_t green_, uint8_t blue_, uint8_t alpha_);
    /** \brief Constructor (opaque)
     *  \param red_ the red component
     *  \param green_ the green component
     *  \param blue_ the blue component
     */
    Color(uint8_t red_, uint8_t green_, uint8_t blue_);
    /** \brief Default constructor (white) */
    Color();
    /** \brief Copy constructor */
    Color(const Color& other);
    uint8_t red;    /**< \brief the red component */
    uint8_t green;  /**< \brief the green component */
    uint8_t blue;   /**< \brief the blue component */
    uint8_t alpha;  /**< \brief the alpha component */
  };
}
#endif
