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
  void drawSplash(Color backgroundColor, Color textColor)
  {
    backgroundColor.alpha = 0;
    textColor.alpha = 0;
    
    Quad background(Pair(), getWindowDimensions(), backgroundColor);

    double min_dim = fmin(getWindowDimensions().x, getWindowDimensions().y);
    Pair scaleMultiplier = Pair(min_dim, min_dim) / Pair(500,500);
    
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
    Pair location = getWindowDimensions() / 2;
    Image wic(location, &wicTexture);
    wic.scale = Pair(10,10) * scaleMultiplier;
    wic.center = wic.getGeoCenter();
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
    location.x += wic.scale.x * wicTexture.getDimensions().x / 2;
    location.y -= wic.scale.y * (wicTexture.getDimensions().y / 2 + 1);
    location = Pair((int) location.x, (int) location.y);
    Image engine(location, &engineTexture);
    engine.scale = Pair(2,2) * scaleMultiplier;
    engine.center = engine.getGeoCenter();
    engine.center.x *= 2;
    engine.drawCentered = true;
    engine.color = textColor;
    
    vector<uint8_t> versionBuffer =
    {
      0,0,0,1,1,0,0,1,1,
      0,0,0,0,1,0,0,0,1,
      0,0,0,0,1,0,0,0,1,
      1,0,1,0,1,0,0,0,1,
      0,1,0,0,1,0,1,0,1,
    };
    Texture versionTexture(versionBuffer, Pair(9,5), Format::Mono,
                           Filter::Nearest, Wrap::Repeat);
    versionTexture.load();
    location.y -= engine.scale.y * (engineTexture.getDimensions().y / 2 + 1);
    location.y = (int) location.y;
    Image version(location, &versionTexture);
    version.scale = Pair(1.5,1.5) * scaleMultiplier;
    version.center = version.getGeoCenter();
    version.center.x *= 2;
    version.center.y *= 2;
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
