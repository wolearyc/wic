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
 * File:    Error.h
 * ----------------------------------------------------------------------------
 */
/** \file */
#ifndef ERROR_H
#define ERROR_H
#include <stdio.h>
#include <string>
using std::string;
namespace wic
{
  /** Runtime exception. */
  class Error : public std::runtime_error
  {
  public:
    /** Constructor.
     *  \param reason the cause of the error
     */
    Error(string reason);
    /** Constructor (unspecified error). */
    Error();
  private:
  };
  /** Internal, unavoidable error. */
  class InternalError : public Error
  {
    using Error::Error;
  };
  /** File loading error. */
  class InvalidFile : public Error
  {
  public:
    /** Constructor.
     *  \param filepath the offending filepath
     */
    InvalidFile(string filepath);
  };
  /** Logically invalid argument error. */
  class InvalidArgument : public std::invalid_argument
  {
  public:
    /** Constructor.
     *  \param name the offending argument's name
     *  \param condition condition satisfied that indicates error
     */
    InvalidArgument(string name, string condition);
    /** Returns the name of the offending argument. */
    string getName();
  private:
    string name;
  };
  /** A nonfatal failure. Failures are present by design. An example might be
   *  a client join failure due to a preexisting ban. 
   */
  class Failure : public Error
  {
    using Error::Error;
  };
}
#endif

