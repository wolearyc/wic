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
 * File:    Pair.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef PAIR_H
#define PAIR_H
#include <cmath>
namespace wic
{
  /** A 2D vector. Pair is the workhorse of wic. A Pair can represent 2D 
   *  dimensions, a 2D vector, 2D scaling information, and, most commonly, 
   *  coordinates. Typical vector arithmetic can be performed between pairs.
   */
  class Pair
  {
  public:
    double x; /**< The x coordinate/x dimension/x component etc. */
    double y; /**< The y coordinate/y dimension/y component etc. */
    /** Constructor.
     *  \param x the x coordinate/x dimension/x component etc.
     *  \param y the y coordinate/y dimension/y component etc.
     */
    Pair(double x, double y);
    /** Constructor.
     *  \param x the x coordinate/x dimension/x component etc.
     *  \param y the y coordinate/y dimension/y component etc.
     */
    Pair(int x, int y);
    /** Default constructor (0,0). */
    Pair();
    /** Copy constructor. */
    Pair(const Pair& other);
    /** Computes the distance to another Pair
     *  \param other another Pair
     *  \return the distance between the Pairs, in whatever units the Pairs
     *         used
     */
    double distance(const Pair& other) const;
    /** Computes the angle to another Pair
     *  \param other another Pair
     *  \return the angle, in radians measured from the positive x axis
     */
    double angle(const Pair& other) const;
    /** Computes the norm.
     *  \return the norm of the Pair
     */
    double norm() const;
    /** Returns a normalized Pair. */
    Pair normalized() const;
    /** Returns a Pair with negative components made positive. */
    Pair abs() const;
    /** Returns the square of the norm. */
    double normSquared() const;
    /** Returns the dot product with another pair
     *  \param other another Pair
     */
    double dot(const Pair& other) const;
    /** Transforms the pair based on a rotation, scale, and center.
     *  \param rotation the rotation
     *  \param scale the scale
     *  \param center the center to rotate and scale around
     */
    void transform(double rotation, Pair scale, Pair center);
    /** Transforms the pair based on a scale and center.
     *  \param scale the scale
     *  \param center the center to rotate and scale around
     */
    void transform(Pair scale, Pair center);
    /** Defines unary negation.
     *  \return the component-wise negation of the pair
     */
    Pair operator-() const;
    /** Defines addition between Pairs.
     *  \param other another Pair
     *  \return the component-wise sum of the Pairs
     */
    Pair operator+(const Pair& other) const;
    /** Defines subraction between Pairs
     *  \param other another Pair
     *  \return the component-wise difference between the Pairs
     */
    Pair operator-(const Pair& other) const;
    /** Defines multiplication between Pairs.
     *  \param other another Pair
     *  \return the component-wise product of the Pairs
     */
    Pair operator*(const Pair& other) const;
    /** Defines division between Pairs.
     *  \param other another Pair
     *  \return the component-wise quotient of the Pairs
     */
    Pair operator/(const Pair& other) const;
    /** Defines addition assignment between Pairs.
     *  \param other another Pair
     *  \return the final added Pair
     */
    Pair operator+=(const Pair& other);
    /** Defines subraction assignment between Pairs.
     *  \param other another Pair
     *  \return the final subtracted Pair
     */
    Pair operator-=(const Pair& other);
    /** Defines multiplication assignment between Pairs.
     *  \param other another Pair
     *  \return the final multiplied Pair
     */
    Pair operator*=(const Pair& other);
    /** Defines division assiment between Pairs.
     *  \param other another Pair
     *  \return the final divided Pair
     */
    Pair operator/=(const Pair& other);
    /** Defines equality operator between Pairs.
     *  \param other another Pair
     *  \return true if both Pairs' respective components are within epsilon
     *          (0.0000001), false otherwise
     */
    bool operator==(const Pair& other) const;
    /** Defines the inequality operator between Pairs.
     *  \param other another Pair
     *  \return false if both Pairs' respective components are within
     *          epsilon (0.0000001), true otherwise
     */
    bool operator!=(const Pair& other) const;
    /** Defines the greater than or equal to operator between Pairs.
     *  \param other another Pair
     *  \return true if the Pair's respective components are both greater
     *         than or equal to the components of the other Pair
     */
    bool operator>=(const Pair& other) const;
    /** Defines the greater than operator between Pairs.
     *  \param other another Pair
     *  \return true if the Pair's respective components are both greater
     *         than to the components of the other Pair
     */
    bool operator>(const Pair& other) const;
    /** Defines the less than or equal to operator between Pairs.
     *  \param other another Pair
     *  \return true if the Pair's respective components are both less than
     *         or equal to the components of the other Pair
     */
    bool operator<=(const Pair& other) const;
    /** Defines the less than operator between Pairs.
     *  \param other another Pair
     *  \return true if the Pair's respective components are both greater
     *         than or equal to the components of the other Pair
     */
    bool operator<(const Pair& other) const;
    /** Defines addition between a Pair and a double.
     *  \param i a double
     *  \return the Pair with the double added to both components
     */
    Pair operator+(const double i) const;
    /** Defines subtraction between a Pair and a double.
     *  \param i a double
     *  \return the Pair with the double subtracted from both components
     */
    Pair operator-(const double i) const;
    /** Defines multiplication between a Pair and a double.
     *  \param i a double
     *  \return the Pair with the double multiplied by both components
     */
    Pair operator*(const double i) const;
    /** Defines division between a Pair and a double.
     * \param i a double
     * \return the Pair with the double divided into  both components
     */
    Pair operator/(const double i) const;
    /** Defines addition assignment between a Pair and a double.
     *  \param i a double
     *  \return the final added Pair
     */
    Pair operator+=(const double i);
    /** Defines subtraction assignment between a Pair and a double.
     *  \param i a double
     *  \return the final subtracted Pair
     */
    Pair operator-=(const double i);
    /** Defines multiplication assignment between a Pair and a double.
     *  \param i a double
     *  \return the final multiplied Pair
     */
    Pair operator*=(const double i);
    /** Defines division assignment between a Pair and a double.
     *  \param i a double
     *  \return the final divided Pair
     */
    Pair operator/=(const double i);
  };
}
#endif
