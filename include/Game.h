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
 * File:    Game.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <unistd.h>
#include <deque>
#include <mutex>
#include <vector>
#include "GLFW/glfw3.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Pair.h"
#include "Error.h"
using std::string;
using std::vector;
namespace wic
{
  /** Enumerates keyboard keys and mouse buttons */
  enum class Key
  {
    SPACE = 32,          /**< the space key */
    APOSTROPHE = 39,     /**< the apostrophe/double quotes key */
    OMMA = 44,           /**< the comma key */
    MINUS = 45,          /**< the minus/underscore key */
    PERIOD = 46,         /**< the period key */
    SLASH = 47,          /**< the slash/question mark key */
    N0 = 48,              /**< the 0 key */
    N1 = 49,              /**< the 1 key */
    N2 = 50,              /**< the 2 key */
    N3 = 51,              /**< the 3 key */
    N4 = 52,              /**< the 4 key */
    N5 = 53,              /**< the 5 key */
    N6 = 54,              /**< the 6 key */
    N7 = 55,              /**< the 7 key */
    N8 = 56,              /**< the 8 key */
    N9 = 57,              /**< the 9 key */
    SEMICOLON = 59,      /**< the semicolon/colon key */
    EQUAL = 61,          /**< the equals/plus key */
    A = 65,              /**< the A key */
    B = 66,              /**< the B key */
    C = 67,              /**< the C key */
    D = 68,              /**< the D key */
    E = 69,              /**< the E key */
    F = 70,              /**< the F key */
    G = 71,              /**< the G key */
    H = 72,              /**< the H key */
    I = 73,              /**< the I key */
    J = 74,              /**< the J key */
    K = 75,              /**< the K key */
    L = 76,              /**< the L key */
    M = 77,              /**< the M key */
    N = 78,              /**< the N key */
    O = 79,              /**< the O key */
    P = 80,              /**< the P key */
    Q = 81,              /**< the Q key */
    R = 82,              /**< the R key */
    S = 83,              /**< the S key */
    T = 84,              /**< the T key */
    U = 85,              /**< the U key */
    V = 86,              /**< the V key */
    W = 87,              /**< the W key */
    X = 88,              /**< the X key */
    Y = 89,              /**< the Y key */
    Z = 90,              /**< the Z key */
    LEFT_BRACKET = 91,   /**< the left bracket/left curly brace key */
    BACKSLASH = 92,      /**< the backslash/vertical bar key */
    RIGHT_BRACKET = 93,  /**< the right bracket/right curly brace key */
    GRAVE_ACCENT = 96,   /**< the grave accent/tilde key */
    ESCAPE = 256,        /**< the escape key */
    ENTER = 257,         /**< the enter key */
    TAB = 258,           /**< the tab key */
    BACKSPACE = 259,     /**< the backspace key */
    INSERT = 260,        /**< the insert key */
    DELETE = 261,        /**< the delete key */
    RIGHT = 262,         /**< the right arrow key */
    LEFT = 263,          /**< the left arrow key */
    DOWN = 264,          /**< the down arrow key */
    UP = 265,            /**< the up arrow key */
    PAGE_UP = 266,       /**< the page up key */
    PAGE_DOWN = 267,     /**< the page down key */
    HOME = 268,          /**< the home key */
    END = 269,           /**< the end key */
    CAPS_LOCK = 280,     /**< the caps lock key */
    SCROLL_LOCK = 281,   /**< the scroll lock key */
    NUM_LOCK = 282,      /**< the num lock key */
    PRINT_SCREEN = 283,  /**< the print screen key */
    PAUSE = 284,         /**< the pause key */
    F1 = 290,            /**< the F1 key */
    F2 = 291,            /**< the F2 key */
    F3 = 292,            /**< the F3 key */
    F4 = 293,            /**< the F4 key */
    F5 = 294,            /**< the F5 key */
    F6 = 295,            /**< the F6 key */
    F7 = 296,            /**< the F7 key */
    F8 = 297,            /**< the F8 key */
    F9 = 298,            /**< the F9 key */
    F10 = 299,           /**< the F10 key */
    F11 = 300,           /**< the F11 key */
    F12 = 301,           /**< the F12 key */
    F13 = 302,           /**< the F13 key */
    F14 = 303,           /**< the F14 key */
    F15 = 304,           /**< the F15 key */
    F16 = 305,           /**< the F16 key */
    F17 = 306,           /**< the F17 key */
    F18 = 307,           /**< the F18 key */
    F19 = 308,           /**< the F19 key */
    F20 = 309,           /**< the F20 key */
    F21 = 310,           /**< the F21 key */
    F22 = 311,           /**< the F22 key */
    F23 = 312,           /**< the F23 key */
    F24 = 313,           /**< the F24 key */
    F25 = 314,           /**< the F25 key */
    KP_0 = 320,          /**< the keypad 0 key */
    KP_1 = 321,          /**< the keypad 1 key */
    KP_2 = 322,          /**< the keypad 2 key */
    KP_3 = 323,          /**< the keypad 3 key */
    KP_4 = 324,          /**< the keypad 4 key */
    KP_5 = 325,          /**< the keypad 5 key */
    KP_6 = 326,          /**< the keypad 6 key */
    KP_7 = 327,          /**< the keypad 7 key */
    KP_8 = 328,          /**< the keypad 8 key */
    KP_9 = 329,          /**< the keypad 9 key */
    KP_DECIMAL = 330,    /**< the keypad decimal key */
    KP_DIVIDE = 331,     /**< the keypad divide key */
    KP_MULTIPLY = 332,   /**< the keypad multiply key */
    KP_SUBTRACT = 333,   /**< the keypad subtract key */
    KP_ADD = 334,        /**< the keypad add key */
    KP_ENTER = 335,      /**< the keypad enter key */
    KP_EQUAL = 336,      /**< the keypad equals key */
    LEFT_SHIFT = 340,    /**< the left shift key */
    LEFT_CONTROL = 341,  /**< the left control key */
    LEFT_ALT = 342,      /**< the left alt key */
    LEFT_SUPER = 343,    /**< the left super key */
    RIGHT_SHIFT = 344,   /**< the right shift key */
    RIGHT_CONTROL = 345, /**< the right control key */
    RIGHT_ALT = 346,     /**< the right alt key */
    RIGHT_SUPER = 347,   /**< the right super key */
    MENU = 348,          /**< the menu key */
    LMB = 350,           /**< the left mouse button */
    RMB = 351,           /**< the right mouse button */
    MMB = 352,           /**< the middle mouse button */
    MB_4 = 353,          /**< mouse button 4 */
    MB_5 = 354,          /**< mouse button 5 */
    MB_6 = 355,          /**< mouse button 6 */
    MB_7 = 356,          /**< mouse button 7 */
    MB_8 = 357           /**< mouse button 8 */
  };
  extern const unsigned CONTINUE;  /**< Code indicating window is still open */
  extern const unsigned TERMINATE; /**< Code indicating window is closed */
  /** Initializes Wic and opens a window.
   *  \param title the desired window title
   *  \param dimensions the desired window dimensions; both components must be
   *         greater than or equal to 32
   *  \param fps the desired number of frames to draw per second; must be > 0
   *  \param resizeable whether or not the window should be resizeable by the
   *         user, ignored if the game is fullscreen
   *  \param fullscreen whether or not the game should run fullscreen
   *  \param samples the number of samples to use with antialiasing, a value of 0
   *         disables antialiasing
   */
  void openWindow(string title, Pair dimensions, unsigned fps, bool resizeable,
                  bool fullscreen, unsigned samples);
  /** Advances to the next frame. This function will wait a certain amount of
   *  time before returning, ensuring that the fps is maintained. This function
   *  performs several system tasks, such as uploading loaded textures to the 
   *  GPU.
   *  \return TERMINATE if the window has been closed and the program should
   *          exit. CONTINUE otherwise.
   */
  unsigned updt();
  /** Forces game to exit. This function closes the window and ensures that
   *  the next call to updt will return TERMINATE.
   */
  void exit();
  /** Deinitializes the (hopefully) closed window and performs cleanup. */
  void cleanUp();
  /** Returns the time since the last updt in seconds. */
  double getDelta();
  /** Returns whether or not a keyboard key/mouse button is being depressed.
   *  \param key the keyboard key/mouse button
   */
  bool isKeyDown(enum Key key);
  /** Returns whether or not a keyboard key/mouse button was pressed since the
   *  last call to updt.
   *  \param key the keyboard key/mouse button
   */
  bool isKeyPressed(enum Key key);
  /** Returns the human-readable keyboard input since the last call to updt */
  string getInput();
  /** Returns the cursor location. */
  Pair getCursorLocation();
  /** Returns the scroll wheel/ball offset since last call to updt. */
  Pair getScrollOffset();
  /** Returns the game time in seconds. */
  double getTime();
  /** Returns the pixel dimensions. */
  Pair getWindowDimensions();
  /** Returns the screen pixel density (pixels/square inch) */
  Pair getPixelDensity();
}
namespace private_wic
{
  void submitTexture(unsigned* textureDataDest, int wrap, int filter,
                     wic::Pair dimensions, vector<uint8_t> buffer);
  wic::Pair getOpenGLVertex(wic::Pair location);
  FT_Library getFTLibrary();
}
#endif
