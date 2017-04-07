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
 * File:    Splash.cpp
 * ----------------------------------------------------------------------------
 */
/** \file */
#include "Splash.h"
namespace wic
{
  void drawSplash(Color textColor)
  {
    Color backgroundColor = Color::Black;
    backgroundColor.alpha = 0;
    textColor.alpha = 0;
    
    Quad background(Pair(), getWindowDimensions(), backgroundColor);

    Pair scaleMultiplier = Pair(2,2);
    
    vector<uint8_t> wicBuffer =
    {
      1,0,0,0,1,0,1,0,0,1,1,1,
      1,0,0,0,1,0,1,0,1,0,0,0,
      1,0,1,0,1,0,1,0,1,0,0,0,
      1,1,0,1,1,0,1,0,1,0,0,0,
      1,0,0,0,1,0,1,0,0,1,1,1
    };
    Texture wicTexture(wicBuffer, Pair(12,5), Format::Mono, Filter::Nearest,
                       Wrap::Repeat);
    wicTexture.load();
    Pair location;
    Image wic(Pair(), &wicTexture);
    wic.scale = Pair(10,10) * scaleMultiplier;
    wic.center.x = wic.getGeoCenter().x * 2;
    wic.location = getWindowDimensions() / 2;
    wic.location.x += wic.scale.x * wic.getGeoCenter().x;
    wic.location.y -= wic.scale.y * wic.getGeoCenter().y;
    wic.drawCentered = true;
    wic.color = textColor;
    
    vector<uint8_t> engineBuffer =
    {
      1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1,
      1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,0,
      1,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,
      1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,
      1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1
    };
    Texture engineTexture(engineBuffer, Pair(23,5),Format::Mono, Filter::Nearest,
                          Wrap::Repeat);
    engineTexture.load();
    Image engine(Pair(), &engineTexture);
    engine.scale = Pair(2,2) * scaleMultiplier;
    engine.center.x = engine.getGeoCenter().x * 2;
    engine.location = wic.location - Pair(0, 25);
    engine.drawCentered = true;
    engine.color = textColor;
    
    vector<uint8_t> versionBuffer =
    {
      0,0,0,0,1,1,0,0,0,1,1,1,
      0,0,0,0,0,1,0,0,0,1,0,1,
      0,1,0,1,0,1,0,0,0,1,0,1,
      1,0,1,0,0,1,0,0,0,1,0,1,
      0,1,0,1,0,1,0,1,0,1,1,1,
    };
    Texture versionTexture(versionBuffer, Pair(12,5), Format::Mono,
                           Filter::Nearest, Wrap::Repeat);
    versionTexture.load();
    Image version(location, &versionTexture);
    version.scale = Pair(1,1) * scaleMultiplier;
    version.center.x = version.getGeoCenter().x * 2;
    version.location = engine.location - Pair(0, 12);
    version.drawCentered = true;
    version.color = textColor;
    
    double time;
    int stage = 0;
    while(updt() == CONTINUE)
    {
      if(!stage)
      {
        background.color.alpha += 3;
        wic.color.alpha        += 3;
        engine.color.alpha     += 3;
        version.color.alpha    += 3;
        if(version.color.alpha == 255)
        {
          time = getTime();
          stage++;
        }
      }
      else if(stage == 1 && getTime() - time >= 1.0)
        stage++;
      else if(stage == 2)
      {
        background.color.alpha -= 3;
        wic.color.alpha        -= 3;
        engine.color.alpha     -= 3;
        version.color.alpha    -= 3;
        if(!version.color.alpha)
          break;
      }
      background.draw();
      wic.draw();
      engine.draw();
      version.draw();
    }
  }
}
