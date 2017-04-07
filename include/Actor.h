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
 * File:    Actor.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef ACTOR_H
#define ACTOR_H
#include "Interfaces.h"
#include "Camera.h"
namespace wic
{
  /** 2D game actor. An actor possesses a location and rotation within the game, 
   *  and is almost always held within a stage. An actor's behavior is defined
   *  through its act method. Additionally, actors can remove themselves 
   *  from the stage.
   */
  class Actor
  : public Locateable, public Rotateable, public Scaleable
  {
  public:
    /** Default constructor. */
    Actor();
    /** Draws the sprite on the screen.
     *  \param spriteLocation a specified location
     *  \param spriteRocation a specified rotation
     *  \param spriteScale a specified scale
     */
    virtual void draw(Pair spriteLocation, double spriteRotation,
                      Pair spriteScale) = 0;
    /** Defines independent behavior. */
    virtual void act() = 0;
    /** Marks actor for removal from the Stage. */
    void markForRemoval();
    /** Returns whether or not the actor is marked for removal. */
    bool shouldRemove() const;
  private:
    bool remove;
  };
  
  /** Implemented by actors that act on i.e. interact with another actor. 
   *  The convention is that actors do not reciprocally act on one another. 
   *  Instead, both actors are updated in a single actor's actOn method. This 
   *  keeps the code simple and maintainable.
   *  \tparam ActorClass actor to act on
   */
  template <class ActorClass> class ActsOn
  {
  public:
    /** Defines interaction with another actor.
     *  \param other another actor.
     */
    virtual void actOn(ActorClass& other) = 0;
  };
}
#endif
