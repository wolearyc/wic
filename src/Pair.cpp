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
 * File:    Pair.cpp
 * ----------------------------------------------------------------------------
 */
#include "Pair.h"
namespace wic
{
  Pair::Pair(double x, double y)
  : x(x), y(y)
  {
  }
  Pair::Pair(int x, int y)
  : Pair((double) x, (double) y)
  {
  }
  Pair::Pair()
  : Pair(0.0,0.0)
  {
  }
  Pair::Pair(const Pair& other) 
  : Pair(other.x, other.y)
  {
  }
  double Pair::distance(const Pair& other) const 
  {
    return(std::sqrt(pow(other.x - x, 2.0) + pow(other.y - y, 2.0)));
  }
  double Pair::angle(const Pair& other) const 
  {
    return(std::atan2(other.y - y, other.x - x));
  }
  double Pair::norm() const 
  {
    return std::sqrt(normSquared());
  }
  Pair Pair::normalized() const
  {
    return Pair(x,y) / norm();
  }
  Pair Pair::abs() const
  {
    return Pair(std::abs(x), std::abs(y));
  }
  double Pair::normSquared() const
  {
    return x*x+y*y;
  }
  double Pair::dot(const Pair& other) const
  {
    return x * other.x + y * other.y;
  }
  void Pair::transform(double rotation, Pair scale, Pair center)
  {
    double cosine = std::cos(rotation);
    double sine = std::sin(rotation);
    Pair tmp = (Pair(x,y) - center) * scale;
    x = center.x + tmp.x * cosine - tmp.y * sine;
    y = center.y + tmp.x * sine   + tmp.y * cosine;
  }
  void Pair::transform(Pair scale, Pair center)
  {
    Pair tmp = (Pair(x,y) - center) * scale;
    x = center.x + tmp.x;
    y = center.y + tmp.y;
  }
  Pair Pair::operator-() const
  {
    return Pair(-x, -y);
  }
  Pair Pair::operator+(const Pair& other) const 
  {
    return(Pair(x + other.x, y + other.y));
  }
  Pair Pair::operator-(const Pair& other) const 
  {
    return(Pair(x - other.x, y - other.y));
  }
  Pair Pair::operator*(const Pair& other) const 
  {
    return(Pair(x * other.x, y * other.y));
  }
  Pair Pair::operator/(const Pair& other) const 
  {
    return(Pair(x / other.x, y / other.y));
  }
  Pair Pair::operator+=(const Pair& other) 
  {
    x += other.x;
    y += other.y;
    return(Pair(x, y));
  }
  Pair Pair::operator-=(const Pair& other) 
  {
    x -= other.x;
    y -= other.y;
    return(Pair(x, y));
  }
  Pair Pair::operator*=(const Pair& other) 
  {
    x *= other.x;
    y *= other.y;
    return(Pair(x, y));
  }
  Pair Pair::operator/=(const Pair& other) 
  {
    x /= other.x;
    y /= other.y;
    return(Pair(x, y));
  }
  bool Pair::operator==(const Pair& other) const
  {
    return(fabs(x - other.x) < 0.0000001 &&
           (fabs(y - other.y) < 0.0000001));
  }
  bool Pair::operator!=(const Pair& other) const 
  {
    return(fabs(x - other.x) >= 0.0000001 &&
           (fabs(y - other.y) >= 0.0000001));
  }
  bool Pair::operator>=(const Pair& other) const
  {
    return(x - other.x >= 0.0000001 &&
           (y - other.y >= 0.0000001));
  }
  bool Pair::operator>(const Pair& other) const
  {
    return(x - other.x > 0.0000001 &&
           (y - other.y > 0.0000001));
  }
  bool Pair::operator<=(const Pair& other) const
  {
    return(x - other.x <= 0.0000001 &&
           (y - other.y <= 0.0000001));
  }
  bool Pair::operator<(const Pair& other) const
  {
    return(x - other.x < 0.0000001 &&
           (y - other.y < 0.0000001));
  }
  Pair Pair::operator+(const double i) const 
  {
    return(Pair(x + i, y + i));
  }
  Pair Pair::operator-(const double i) const 
  {
    return(Pair(x - i, y - i));
  }
  Pair Pair::operator*(const double i) const 
  {
    return(Pair(x * i, y * i));
  }
  Pair Pair::operator/(const double i) const 
  {
    return(Pair(x / i, y / i));
  }
  Pair Pair::operator+=(const double i) 
  {
    x += i;
    y += i;
    return(Pair(x, y));
  }
  Pair Pair::operator-=(const double i) 
  {
    x -= i;
    y -= i;
    return(Pair(x, y));
  }
  Pair Pair::operator*=(const double i) 
  {
    x *= i;
    y *= i;
    return(Pair(x, y));
  }
  Pair Pair::operator/=(const double i) 
  {
    x /= i;
    y /= i;
    return(Pair(x, y));
  }
}
