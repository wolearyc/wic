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
 * File:    Stage.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef STAGE_H
#define STAGE_H
#include <vector>
#include <initializer_list>
#include "Interfaces.h"
#include "Camera.h"
#include "Actor.h"
using std::vector;
namespace wic
{
  /** An environment shared by actors. */
  class Stage
  {
  public:
    /** Updates actors. This includes handing actions of singular actors and
     *  interactions between actors.
     */
    virtual void updt() = 0;
    /** Draws actors.
     *  \param camera the camera through which to view
     */
    virtual void draw(const Camera& camera) = 0;
  };
  
  /** Indicates that a Stage can contain actors of a certain type. 
   *  \tparam ActorClass an actor class
   */
  template <class ActorClass> class Contains
  {
  public:
    /** Draws all the actors.
     *  \param camera the camera through which to view
     */
    void drawAll(const Camera& camera)
    {
      for(auto actor = actors.begin() ; actor != actors.end(); ++actor)
        actor->draw(camera.getDrawLocation(actor->location),
                    camera.getDrawRotation(actor->rotation),
                    camera.getDrawScale(actor->scale));
    }
    /** Updates actors and removes those marked for removal. This method 
     *  handles individual actions. 
     */
    void handle()
    {
      for(auto actor = actors.begin() ; actor != actors.end(); ++actor)
        actor->act();
      
      std::remove_if(actors.begin(), actors.end(),
                     [](const ActorClass& actor)
                     { return actor.shouldRemove(); });
    }
  protected:
    vector<ActorClass> actors; /**< List of actors */
  };
  template <class ActorClass, class ActingOn> class ContainsInteracting
  {
  public:
    void handle(vector<ActorClass> actors, vector<ActingOn> others)
    {
      for(auto actor = actors.begin(); actor != actors.end(); ++actor)
      {
        for(auto other = others.begin(); other != others.end(); ++other)
        {
          actor->actOn(*other);
        }
      }
    }
  };
}
#endif
