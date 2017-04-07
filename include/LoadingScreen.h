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
 * File:    LoadingScreen.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
#include <atomic>
#include "wic.h"
using std::atomic;
namespace wic
{
  /** Displays a minimal loading screen with a progress bar. */
  class LoadingScreen
  {
  public:
    /** Constructor. */
    LoadingScreen();
    /** Displays the loading screen. This function returns when the progress is
     *  set to 100%. 
     */
    void display();
    /** Safely sets the progress (from any thread).
     *  \param progress the progress percentage; must be <= 100
     */
    void setProgress(unsigned progress);
  private:
    Quad outerFrame;
    Quad innerFrame;
    Quad bar;
    atomic<unsigned> progress;
  };
}
#endif
