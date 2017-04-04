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
   *  and is almost always held within a Map. An actor's behavior is defined
   *  through its act method. 
   */
  class Actor
  : public Locateable, public Rotateable
  {
  public:
    /** Default constructor. */
    Actor();
    /** Draws actor's sprite. Implemented in subclasses.
     *  \param game the game
     *  \param spriteLocation a specified location
     *  \param spriteRocation a specified rotation
     *  \param spriteScale a specified scale
     */
    virtual void draw(const Game& game, Pair spriteLocation,
                      double spriteRotation, Pair spriteScale) = 0;
    /** Defines the actors behavior. Implemented in subclasses.
     *  \param game the game
     */
    virtual void act(const Game& game) = 0;
    /** Marks the actor for removal from the map. */
    void markForRemoval();
    /** Returns whether or not to remove from the map */
    bool shouldRemove() const;
  private:
    bool remove;
  };
  
  /** Implemented by actors that interact, or act on, another actor. */
  template <class ActorClass> class ActsOn
  {
  public:
    /** Defines the actor's interaction with another actor.
     *  \param other another actor.
     */
    virtual void actOn(ActorClass& other) = 0;
  };
}
#endif
