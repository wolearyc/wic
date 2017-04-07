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
 * File:    Camera.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef CAMERA_H
#define CAMERA_H
#include "Interfaces.h"
namespace wic
{
  /** Specifies how a Stage is rendered. Like a real camera, Camera can move,
   *  rotate, and zoom in. A zoom of 1 indicates no zoom, while zoom > 1 and
   *  zoom < 1 indicates zooming in and out respectively.
   */
  class Camera
  : public Locateable, public Rotateable
  {
  public:
    /** Constructor.
     *  \param location the location
     *  \param rotation the rotation
     *  \param zoom the zoom; must be > 0
     */
    Camera(Pair location, double rotation, double zoom);
    /** Returns the drawing location of an actor.
     *  \param actorLocation an actor location
     */
    Pair getDrawLocation(Pair actorLocation) const;
    /** Returns the drawing rotation of an actor.
     *  \param actorRotation an actor rotation
     */
    double getDrawRotation(double actorRotation) const;
    /** Returns the drawing scale of an actor.
     *  \param actorScale an actor scale
     */
    Pair getDrawScale(Pair actorScale) const;
    /** Returns the current zoom. */
    double getZoom() const;
    /** Sets the zoom. 
     *  \param zoom the zoom; must be > 0
     */
    void setZoom(double zoom);
  private:
    double zoom;
  };
}
#endif
