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
 * File:    LoadingScreen.cpp
 * ----------------------------------------------------------------------------
 */
#include "LoadingScreen.h"
namespace wic
{
  LoadingScreen::LoadingScreen()
  : progress(0)
  {
    outerFrame = Quad(getWindowDimensions() / 2, Pair(100,20), Color::WicGray);
    innerFrame = Quad(getWindowDimensions() / 2, Pair(96, 16), Color::Black);
    Pair barLocation = innerFrame.location - innerFrame.getGeoCenter() + 1;
    bar = Quad(barLocation, Pair(0,14), Color::WicOrange);
    outerFrame.drawCentered = true;
    outerFrame.center = outerFrame.getGeoCenter();
    innerFrame.drawCentered = true;
    innerFrame.center = innerFrame.getGeoCenter();
  }
  void LoadingScreen::display()
  {
    bool lighten = true;
    int stage = 0;
    while(updt() == CONTINUE && progress < 100)
    {
      bar.dimensions.x = progress;
      
      // Pulse bar
      if(lighten)
      {
        bar.color.lighten(3);
        stage+=3;
      }
      if(!lighten)
      {
        bar.color.darken(1);
        stage--;
      }
      if(stage == 15)
        lighten = false;
      if(stage == -15)
        lighten = true;
      
      outerFrame.draw();
      innerFrame.draw();
      bar.draw();
    }
  }
  void LoadingScreen::setProgress(unsigned progress)
  {
    if(progress > 100)
      throw InvalidArgument("progress", "> 100");
    
    this->progress = progress;
  }
}
