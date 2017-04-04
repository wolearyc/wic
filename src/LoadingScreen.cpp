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
  LoadingScreen::LoadingScreen(const Game& game)
  : progress(0)
  {
    frame = Quad(game.getDimensions() / 2, Pair(100,20), Color::WicGray);
    bar = Quad(frame.location - frame.getGeoCenter(), Pair(0,20),
               Color::WicOrange);
    frame.drawCentered = true;
    frame.center = frame.getGeoCenter();
  }
  void LoadingScreen::display(Game& game)
  {
    bool lighten = true;
    int stage = 0;
    const int MAX_STAGE = 15;
    while(game.updt() == CONTINUE && progress < 100)
    {
      bar.dimensions.x = progress;
      
      // Pulse bar
      if(lighten)
      {
        bar.color.lighten(1);
        stage++;
      }
      if(!lighten)
      {
        bar.color.darken(1);
        stage--;
      }
      if(stage == MAX_STAGE)
        lighten = false;
      if(stage == -MAX_STAGE)
        lighten = true;
      
      frame.draw(game);
      bar.draw(game);
    }
  }
  void LoadingScreen::setProgress(unsigned progress)
  {
    if(progress > 100)
      throw InvalidArgument("progress", "> 100");
    
    this->progress = progress;
  }
}
