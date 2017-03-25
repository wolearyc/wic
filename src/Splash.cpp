/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for Mac OSX written in C++
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
  const Color SPLASH_backgroundColor = {63,63,63,255};
  const Color SPLASH_textColor = {253,144,11,255};

  void drawSplash(Color backgroundColor, Color textColor, Game& game)
  {
    
    backgroundColor.alpha = 0;
    textColor.alpha = 0;
    
    Quad background = Quad(Pair(), game.getDimensions(), backgroundColor);

    double min_dim = fmin(game.getDimensions().x, game.getDimensions().y);
    Pair scale_multiplier = Pair(min_dim, min_dim) / Pair(500,500);
    
    unsigned char wic_buffer[] =
    {
      1,0,0,0,1,0,1,0,0,1,1,1,
      1,0,0,0,1,0,1,0,1,0,0,0,
      1,0,1,0,1,0,1,0,1,0,0,0,
      1,1,0,1,1,0,1,0,1,0,0,0,
      1,0,0,0,1,0,1,0,0,1,1,1
    };
    Texture wic_texture = Texture(wic_buffer, Pair(12,5),
                                  WIC_MONO, WIC_NEAREST, WIC_REPEAT);
    

    Pair location = game.getDimensions() / 2;
    Image wic = Image(location, &wic_texture);
    
    wic.scale = Pair(10,10) * scale_multiplier;
    wic.center = wic.getGeoCenter();
    wic.drawCentered = true;
    wic.color = textColor;
    unsigned char engine_buffer[] =
    {
      1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1,
      1,0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,0,
      1,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,1,
      1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,
      1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1
    };
    Texture engine_texture = Texture(engine_buffer, Pair(23,5),
                                                           WIC_MONO,
                                                           WIC_NEAREST,
                                                           WIC_REPEAT);

    
    location.x += wic.scale.x * wic_texture.getDimensions().x / 2;
    location.y -= wic.scale.y * (wic_texture.getDimensions().y / 2 + 1);
    location = Pair((int) location.x, (int) location.y);
    Image engine = Image(location, &engine_texture);

    engine.scale = Pair(2,2) * scale_multiplier;
    engine.center = engine.getGeoCenter();
    engine.center.x *= 2;
    engine.drawCentered = true;
    engine.color = textColor;
    unsigned char version_buffer[] =
    {
      0,0,0,1,1,0,0,1,1,
      0,0,0,0,1,0,0,0,1,
      0,0,0,0,1,0,0,0,1,
      1,0,1,0,1,0,0,0,1,
      0,1,0,0,1,0,1,0,1,
    };
    Texture version_texture = Texture(version_buffer, Pair(9,5),
                                                            WIC_MONO,
                                                            WIC_NEAREST,
                                                            WIC_REPEAT);

    
    location.y -= engine.scale.y * (engine_texture.getDimensions().y / 2 + 1);
    location.y = (int) location.y;
    Image version = Image(location, &version_texture);

    version.scale = Pair(1.5,1.5) * scale_multiplier;
    version.center = version.getGeoCenter();
    version.center.x *= 2;
    version.center.y *= 2;
    version.drawCentered = true;
    version.color = textColor;
    double time;
    int stage = 0;
    while(game.updt() == CONTINUE)
    {
      if(!stage)
      {
        background.color.alpha += 3;
        wic.color.alpha        += 3;
        engine.color.alpha     += 3;
        version.color.alpha    += 3;
        if(version.color.alpha == 255)
        {
          time = game.getTime();
          stage++;
        }
      }
      else if(stage == 1 && game.getTime() - time >= 1.0)
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
      background.draw(game);
      wic.draw(game);
      engine.draw(game);
      version.draw(game);
    }
  }
}
