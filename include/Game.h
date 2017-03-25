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
 * File:    Game.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "GLFW/glfw3.h"
#include "FreeType/ft2build.h"
#include FT_FREETYPE_H
#include "Pair.h"
#include "Error.h"
using std::string;
namespace wic
{
  /** \brief defines constants for keyboard keys and mouse buttons */
  enum Key
  {
    WIC_SPACE = 32,          /**< the space key */
    WIC_APOSTROPHE = 39,     /**< the apostrophe/double quotes key */
    WK_COMMA = 44,           /**< the comma key */
    WIC_MINUS = 45,          /**< the minus/underscore key */
    WIC_PERIOD = 46,         /**< the period key */
    WIC_SLASH = 47,          /**< the slash/question mark key */
    WIC_0 = 48,              /**< the 0 key */
    WIC_1 = 49,              /**< the 1 key */
    WIC_2 = 50,              /**< the 2 key */
    WIC_3 = 51,              /**< the 3 key */
    WIC_4 = 52,              /**< the 4 key */
    WIC_5 = 53,              /**< the 5 key */
    WIC_6 = 54,              /**< the 6 key */
    WIC_7 = 55,              /**< the 7 key */
    WIC_8 = 56,              /**< the 8 key */
    WIC_9 = 57,              /**< the 9 key */
    WIC_SEMICOLON = 59,      /**< the semicolon/colon key */
    WIC_EQUAL = 61,          /**< the equals/plus key */
    WIC_A = 65,              /**< the A key */
    WIC_B = 66,              /**< the B key */
    WIC_C = 67,              /**< the C key */
    WIC_D = 68,              /**< the D key */
    WIC_E = 69,              /**< the E key */
    WIC_F = 70,              /**< the F key */
    WIC_G = 71,              /**< the G key */
    WIC_H = 72,              /**< the H key */
    WIC_I = 73,              /**< the I key */
    WIC_J = 74,              /**< the J key */
    WIC_K = 75,              /**< the K key */
    WIC_L = 76,              /**< the L key */
    WIC_M = 77,              /**< the M key */
    WIC_N = 78,              /**< the N key */
    WIC_O = 79,              /**< the O key */
    WIC_P = 80,              /**< the P key */
    WIC_Q = 81,              /**< the Q key */
    WIC_R = 82,              /**< the R key */
    WIC_S = 83,              /**< the S key */
    WIC_T = 84,              /**< the T key */
    WIC_U = 85,              /**< the U key */
    WIC_V = 86,              /**< the V key */
    WIC_W = 87,              /**< the W key */
    WIC_X = 88,              /**< the X key */
    WIC_Y = 89,              /**< the Y key */
    WIC_Z = 90,              /**< the Z key */
    WIC_LEFT_BRACKET = 91,   /**< the left bracket/left curly brace key */
    WIC_BACKSLASH = 92,      /**< the backslash/vertical bar key */
    WIC_RIGHT_BRACKET = 93,  /**< the right bracket/right curly brace key */
    WIC_GRAVE_ACCENT = 96,   /**< the grave accent/tilde key */
    WIC_ESCAPE = 256,        /**< the escape key */
    WIC_ENTER = 257,         /**< the enter key */
    WIC_TAB = 258,           /**< the tab key */
    WIC_BACKSPACE = 259,     /**< the backspace key */
    WIC_INSERT = 260,        /**< the insert key */
    WIC_DELETE = 261,        /**< the delete key */
    WIC_RIGHT = 262,         /**< the right arrow key */
    WIC_LEFT = 263,          /**< the left arrow key */
    WIC_DOWN = 264,          /**< the down arrow key */
    WIC_UP = 265,            /**< the up arrow key */
    WIC_PAGE_UP = 266,       /**< the page up key */
    WIC_PAGE_DOWN = 267,     /**< the page down key */
    WIC_HOME = 268,          /**< the home key */
    WIC_END = 269,           /**< the end key */
    WIC_CAPS_LOCK = 280,     /**< the caps lock key */
    WIC_SCROLL_LOCK = 281,   /**< the scroll lock key */
    WIC_NUM_LOCK = 282,      /**< the num lock key */
    WIC_PRINT_SCREEN = 283,  /**< the print screen key */
    WIC_PAUSE = 284,         /**< the pause key */
    WIC_F1 = 290,            /**< the F1 key */
    WIC_F2 = 291,            /**< the F2 key */
    WIC_F3 = 292,            /**< the F3 key */
    WIC_F4 = 293,            /**< the F4 key */
    WIC_F5 = 294,            /**< the F5 key */
    WIC_F6 = 295,            /**< the F6 key */
    WIC_F7 = 296,            /**< the F7 key */
    WIC_F8 = 297,            /**< the F8 key */
    WIC_F9 = 298,            /**< the F9 key */
    WIC_F10 = 299,           /**< the F10 key */
    WIC_F11 = 300,           /**< the F11 key */
    WIC_F12 = 301,           /**< the F12 key */
    WIC_F13 = 302,           /**< the F13 key */
    WIC_F14 = 303,           /**< the F14 key */
    WIC_F15 = 304,           /**< the F15 key */
    WIC_F16 = 305,           /**< the F16 key */
    WIC_F17 = 306,           /**< the F17 key */
    WIC_F18 = 307,           /**< the F18 key */
    WIC_F19 = 308,           /**< the F19 key */
    WIC_F20 = 309,           /**< the F20 key */
    WIC_F21 = 310,           /**< the F21 key */
    WIC_F22 = 311,           /**< the F22 key */
    WIC_F23 = 312,           /**< the F23 key */
    WIC_F24 = 313,           /**< the F24 key */
    WIC_F25 = 314,           /**< the F25 key */
    WIC_KP_0 = 320,          /**< the keypad 0 key */
    WIC_KP_1 = 321,          /**< the keypad 1 key */
    WIC_KP_2 = 322,          /**< the keypad 2 key */
    WIC_KP_3 = 323,          /**< the keypad 3 key */
    WIC_KP_4 = 324,          /**< the keypad 4 key */
    WIC_KP_5 = 325,          /**< the keypad 5 key */
    WIC_KP_6 = 326,          /**< the keypad 6 key */
    WIC_KP_7 = 327,          /**< the keypad 7 key */
    WIC_KP_8 = 328,          /**< the keypad 8 key */
    WIC_KP_9 = 329,          /**< the keypad 9 key */
    WIC_KP_DECIMAL = 330,    /**< the keypad decimal key */
    WIC_KP_DIVIDE = 331,     /**< the keypad divide key */
    WIC_KP_MULTIPLY = 332,   /**< the keypad multiply key */
    WIC_KP_SUBTRACT = 333,   /**< the keypad subtract key */
    WIC_KP_ADD = 334,        /**< the keypad add key */
    WIC_KP_ENTER = 335,      /**< the keypad enter key */
    WIC_KP_EQUAL = 336,      /**< the keypad equals key */
    WIC_LEFT_SHIFT = 340,    /**< the left shift key */
    WIC_LEFT_CONTROL = 341,  /**< the left control key */
    WIC_LEFT_ALT = 342,      /**< the left alt key */
    WIC_LEFT_SUPER = 343,    /**< the left super key */
    WIC_RIGHT_SHIFT = 344,   /**< the right shift key */
    WIC_RIGHT_CONTROL = 345, /**< the right control key */
    WIC_RIGHT_ALT = 346,     /**< the right alt key */
    WIC_RIGHT_SUPER = 347,   /**< the right super key */
    WIC_MENU = 348,          /**< the menu key */
    WIC_LMB = 350,           /**< the left mouse button */
    WIC_RMB = 351,           /**< the right mouse button */
    WIC_MMB = 352,           /**< the middle mouse button */
    WIC_MB_4 = 353,          /**< mouse button 4 */
    WIC_MB_5 = 354,          /**< mouse button 5 */
    WIC_MB_6 = 355,          /**< mouse button 6 */
    WIC_MB_7 = 356,          /**< mouse button 7 */
    WIC_MB_8 = 357           /**< mouse button 8 */
  };

  extern const unsigned CONTINUE;
  extern const unsigned TERMINATE;
  /** \brief represents the entire  game
   *
   *  A Game should be initialized via wic_init_game. A Game should
   *  eventually be deallocated via wic_free_game.
   */
  class Game
  {
  friend class Texture;
  friend class Font;
  public:
    /** \brief initializes the game and creates a window
     *  \param title the desired window title; must contain one or more characters
     *  \param dimensions the desired window dimensions; both components must be
     *         greater than or equal to 32
     *  \param fps the desired number of frames to draw per second; must be > 0
     *  \param resizeable whether or not the window should be resizeable by the
     *         user, ignored if the game is fullscreen
     *  \param fullscreen whether or not the game should run fullscreen
     *  \param samples the number of samples to use with antialiasing, a value of 0
     *         disables antialiasing
     *  \return a valid pointer to a Game on success, null on failure
     */
    Game(string title, Pair dimensions, unsigned fps,
         bool resizeable, bool fullscreen, unsigned samples);
    /** \brief frees the game
     *
     *  This function should only be called after the window has been closed and the
     *  program is about to be terminated.
     *  \return true on success, false on failure
     */
    ~Game();
    /** \brief flips the window buffers and times game updates
     *
     *  This function will wait a certain amount of time before returning, assuming
     *  there was no error, This mechanism ensures that the fps is maintained.
     *  \return WIC_GAME_CONTINUE if the game should be updated, WIC_GAME_TERMINATE
     *          if the game should not be updated (for example when the window is
     *          closed), 0 on failure
     */
    unsigned updt();
    /** \brief closes the window
     *
     *  When this function is called, the window will be closed and updt_game will
     *  return WIC_GAME_TERMINATE the next time it is called.
     *  \return true on success, false on failure
     */
    bool exit();
    /** \brief fetches the time since the last update in seconds
     *  \return the time since the last update in seconds, -1 on failure
     */
    double getDelta();
    /** \brief determines whether or not a keyboard key/mouse button is being
     *         depressed
     *  \param key ID of the keyboard key/mouse button
     *  \return true if the keyboard key/mouse button is being depressed, false
     *          otherwise
     */
    bool isKeyDown(enum Key key);
    /** \brief determines whether or not a keyboard key/mouse button was pressed
     *         since the last game update
     *  \param key ID of the keyboard key/mouse button
     *  \return true if the keyboard key/mouse button was pressed since the last
     *          game update, false otherwise
     */
    bool isKeyPressed(enum Key key);
    /** \brief retrieves the keyboard input (human-readable) since the last game
     *        update
     *  \return a automatic unsigned char array of length 100, the input can be
     *         found at the beginning of the array, the rest of the array is filled
     *         with 0s
     */
    string getInput();
    /** \brief retrieves the cursor location
     *  \param game the game
     *  \return the cursor location, measured in pixels from the lower-left corner
     *          of the window
     */
    Pair getCursorLocation(Game* game);
    /** \brief retrieves the scroll wheel/ball offset since the last game update
     *  \return the scroll wheel/ball offset since the last game update
     */
    Pair getScrollOffset();
    /** \brief retrieves the time since init_game was called
     *  \return the time since init_game was called in seconds
     */
    double getTime();
    Pair getDimensions();
  private:
    GLFWwindow* window_;
    Pair dimensions_;
    Pair pixelDensity_;
    double secondsPerFrame_;
    double previousTime_;
    double delta_;
    FT_Library FTLibrary_;
  };
  Pair convertLocation(Pair location, Pair dimensions);
  
}
#endif
