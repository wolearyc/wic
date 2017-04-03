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
 * File:    Map.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef MAP_H
#define MAP_H
#include <vector>
#include "Interfaces.h"
#include "Camera.h"
#include "Actor.h"
using std::vector;
namespace wic
{
  /** An environment shared by actors. */
  class Map
  {
  public:
    /** Updates actors. This includes handing actions of singular actors and
     *  interactions between actors.
     *  \param game the game
     */
    virtual void updtActors(const Game& game) = 0;
    /** Draws actors.
     *  \param game the game
     *  \param camera the camera through which to draw the map
     */
    virtual void drawActors(const Game& game, const Camera& camera) = 0;
  };
  
  /** Template indicating that a map contains a certain type of actor. This
   *  template is defined to reduce the amount of code required. 
   */
  template <class ActorClass> class Contains
  {
  public:
    /** Draws all contained.
     *  \param game the game
     *  \param camera the camera through which to draw the map
     */
    void drawAll(const Game& game, const Camera& camera)
    {
      for(auto actor = actors.begin() ; actor != actors.end(); ++actor)
        actor->draw(game, camera.getDrawLocation(actor->location),
                    camera.getDrawRotation(actor->rotation),
                    camera.getDrawScale(Pair(1,1)));
    }
    /** Updates actors and removes those marked for removal. This helpful 
     *  method handles actions of singular actors, but does not handle 
     *  interactions between actors. As of now, interactions must be handled
     *  manually within Map's updtActors function.
     *  \param game the game
     */
    void updtAll(const Game& game)
    {
      for(auto actor = actors.begin() ; actor != actors.end(); ++actor)
        actor->act(game);
      
      std::remove_if(actors.begin(), actors.end(),
                     [](const ActorClass& actor)
                     { return actor.shouldRemove(); });
    }
  protected:
    vector<ActorClass> actors; /**< Currently active actors */
  };
  
}
#endif
