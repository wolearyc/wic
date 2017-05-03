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
 *
 * NOTE: Collision detection code is adapted from Randy Gaul's tinyc2 library
 * ----------------------------------------------------------------------------
 * File:    Hitbox.cpp
 * ----------------------------------------------------------------------------
 */
/** \file */
#include "Hitbox.h"
namespace wic
{
  // Utility functions.
  Pair max(Pair a, Pair b)
  {
    return Pair(std::max(a.x, b.x), std::max(a.y, b.y));
  }
  Pair min(Pair a, Pair b)
  {
    return Pair(std::min(a.x, b.x), std::min(a.y, b.y));
  }
  Pair clamp(Pair a, Pair lo, Pair hi)
  {
    return max(lo, min(a, hi));
  }
  
  Contact::Contact(Pair point, double depth, Pair normal)
  : intersecting(true), point(point), depth(depth), normal(normal)
  {
  }
  bool Contact::isIntersecting() const
  {
    return intersecting;
  }
  Pair Contact::getPoint() const
  {
    return point;
  }
  double Contact::getDepth() const
  {
    return depth;
  }
  Pair Contact::getNormal() const
  {
    return normal;
  }
  
  HitBoxAligned::HitBoxAligned(Pair location, Pair dimensions)
  : Locateable(location), Scaleable(), centered(false), dimensions(dimensions)
  {
  }
  HitBoxAligned::HitBoxAligned()
  : HitBoxAligned(Pair(), Pair(50,50))
  {
  }
  HitBoxAligned::HitBoxAligned(const HitBoxAligned& other)
  : Locateable(other), Scaleable(other), centered(other.centered),
    dimensions(other.dimensions)
  {
  }
  bool HitBoxAligned::isIntersecting(const HitBoxAligned& other) const
  {
    Pair max = getMax();
    Pair min = getMin();
    Pair otherMax = other.getMax();
    Pair otherMin = other.getMin();
    
    bool d0 = otherMax.x < min.x;
    bool d1 = max.x < otherMin.x;
    bool d2 = otherMax.y < min.y;
    bool d3 = max.y < otherMin.y;
    return !(d0 | d1 | d2 | d3);
  }
  bool HitBoxAligned::isIntersecting(const HitCircle& other) const
  {
    Pair L = clamp(other.location, getMin(), getMax());
    Pair ab = other.location - L;
    double d2 = ab.normSquared();
    return d2 < other.radius * other.radius;
  }
  Contact HitBoxAligned::getContact(const HitBoxAligned& other) const
  {
    Pair mid_a = (getMin() + getMax()) * 0.5;
    Pair mid_b = (other.getMin() + other.getMax()) * 0.5;
    Pair tmp = (getMax() - getMin()) * 0.5;
    Pair eA = tmp.abs();
    tmp = (other.getMax() - other.getMin()) * 0.5;
    Pair eB = tmp.abs();
    Pair d = mid_b - mid_a;
    
    // calc overlap on x and y axes
    float dx = eA.x + eB.x - std::abs(d.x);
    if ( dx < 0 ) return Contact();
    float dy = eA.y + eB.y - std::abs(d.y);
    if ( dy < 0 ) return Contact();
    
    Pair n;
    float depth;
    Pair p;
    
    // x axis overlap is smaller
    if ( dx < dy )
    {
      depth = dx;
      if ( d.x < 0 )
      {
        n = Pair(-1.0, 0.0);
        p = mid_a - Pair(eA.x, 0.0);
      }
      else
      {
        n = Pair(1.0, 0.0);
        p = mid_a + Pair(eA.x, 0.0);
      }
    }
    
    // y axis overlap is smaller
    else
    {
      depth = dy;
      if ( d.y < 0 )
      {
        n = Pair(0.0, -1.0);
        p = mid_a - Pair(0.0, eA.y);
      }
      else
      {
        n = Pair(0.0, 1.0);
        p = mid_a + Pair(0.0, eA.y);
      }
    }
    return Contact(p, depth, n);
  }
  Contact HitBoxAligned::getContact(const HitCircle& other) const
  {
    Pair L = clamp(other.location, getMin(), getMax());
    Pair ab = L - other.location;
    double d2 = ab.normSquared();
    double r2 = other.radius * other.radius;
    if (d2 < r2)
    {
      // shallow (center of circle not inside of AABB)
      if ( d2 != 0 )
      {
        double d = std::sqrt(d2);
        Pair n = ab.normalized(); // change
        return Contact(other.location + n * -d, other.radius - d, n);
      }
      
      // deep (center of circle inside of AABB)
      // clamp circle's center to edge of AABB, then form the manifold
      else
      {
        Pair mid = (getMin() + getMax()) * 0.5;
        Pair e = (getMax() - getMin()) * 0.5;
        Pair d = other.location - mid;
        Pair abs_d = Pair(std::abs(d.x), std::abs(d.y));
        Pair n;
        Pair p = other.location;
        float depth;
        if ( abs_d.x > abs_d.y )
        {
          if ( d.x < 0 )
          {
            n = Pair(-1.0, 0.0);
            p.x = mid.x - e.x;
          }
          
          else
          {
            n = Pair(1.0, 0.0);
            p.x = mid.x + e.x;
          }
          depth = e.x - abs_d.x;
        }
        else
        {
          if ( d.y < 0 )
          {
            n = Pair(0.0, -1.0);
            p.y = mid.y - e.y;
          }
          
          else
          {
            n = Pair(0.0, 1.0f);
            p.y = mid.y + e.y;
          }
          depth = e.y - abs_d.y;
        }
        return Contact(p, depth, n);
      }
    }
    return Contact();
  }
  Pair HitBoxAligned::getGeoCenter() const
  {
    return dimensions / 2;
  }
  Pair HitBoxAligned::getMin() const
  {
    Pair vertex;
    vertex.transform(scale, center);
    if(centered)
      vertex -= center;
    return vertex;
  }
  Pair HitBoxAligned::getMax() const
  {
    Pair vertex = dimensions;
    vertex.transform(scale, center);
    if(centered)
      vertex -= center;
    return vertex;
  }
  
  HitCircle::HitCircle(Pair location, double radius)
  : Locateable(location), Scaleable(), centered(false), radius(radius)
  {
  }
  HitCircle::HitCircle()
  : HitCircle(Pair(), 32)
  {
  }
  HitCircle::HitCircle(const HitCircle& other)
  : Locateable(other), Scaleable(other), centered(other.centered),
    radius(other.radius)
  {
  }
  bool HitCircle::isIntersecting(const HitBoxAligned& other) const
  {
    Pair L = clamp(location, other.getMin(), other.getMax());
    Pair ab = location - L;
    double d2 = ab.normSquared();
    return d2 < radius*radius;
  }
  bool HitCircle::isIntersecting(const HitCircle& other) const
  {
    Pair displacement = location - other.location;
    double cutoff = radius + other.radius;
    cutoff *= cutoff;
    return displacement.normSquared() < cutoff;
  }
  Contact HitCircle::getContact(const HitBoxAligned& other) const
  {
    Pair L = clamp(location, other.getMin(), other.getMax());
    Pair ab = L - location;
    double d2 = ab.normSquared();
    double r2 = radius * radius;
    if (d2 < r2)
    {
      // shallow (center of circle not inside of AABB)
      if ( d2 != 0 )
      {
        double d = std::sqrt(d2);
        Pair n = ab.normalized(); // change
        return Contact(location + n * -d, radius - d, n);
      }
      
      // deep (center of circle inside of AABB)
      // clamp circle's center to edge of AABB, then form the manifold
      else
      {
        Pair mid = (other.getMin() + other.getMax()) * 0.5;
        Pair e = (other.getMax() - other.getMin()) * 0.5;
        Pair d = location - mid;
        Pair abs_d = Pair(std::abs(d.x), std::abs(d.y));
        Pair n;
        Pair p = location;
        float depth;
        if ( abs_d.x > abs_d.y )
        {
          if ( d.x < 0 )
          {
            n = Pair(-1.0, 0.0);
            p.x = mid.x - e.x;
          }
          
          else
          {
            n = Pair(1.0, 0.0);
            p.x = mid.x + e.x;
          }
          depth = e.x - abs_d.x;
        }
        else
        {
          if ( d.y < 0 )
          {
            n = Pair(0.0, -1.0);
            p.y = mid.y - e.y;
          }
          
          else
          {
            n = Pair(0.0, 1.0f);
            p.y = mid.y + e.y;
          }
          depth = e.y - abs_d.y;
        }
        return Contact(p, depth, n);
      }
    }
    return Contact();
  }
  Contact HitCircle::getContact(const HitCircle& other) const
  {
    Pair d = other.location - location;
    double d2 = d.normSquared();
    double r = radius + other.radius;
    if (d2 < r * r)
    {
      float l = std::sqrt(d2);
      return Contact((location+other.location) * 0.5, r-l,
                     l != 0 ? (d * 1.0f / l) : Pair(0,1));
    }
    return Contact();
  }
  Pair HitCircle::getGeoCenter() const
  {
    return Pair(radius, radius);
  }
  
}
