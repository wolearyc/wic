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
  /** \brief General runtime exception */
  class Error : public std::runtime_error
  {
  public:
    /** \brief Constructor
     * \param reason the reason for the error
     */
    Error(string reason);
    /** \brief Constructor (when error is unspecified) */
    Error();
  private:
  };
  /** \brief Internal, unavoidable error */
  class InternalError : public Error
  {
    using Error::Error;
  };
  /** \brief thrown when a file cannot be found or loaded */
  class InvalidFile : public Error
  {
  public:
    /** \brief Constructor
     *  \param filepath the offending filepath
     */
    InvalidFile(string filepath);
  };
  /** \brief thrown when an argument is logically invalid */
  class InvalidArgument : public std::invalid_argument
  {
  public:
    /** \brief Constructor
     *  \param name the offending argument's name
     *  \param condition condition on argument that caused the error
     */
    InvalidArgument(string name, string condition);
    string name();
  private:
    string name_;
  };
  /** \brief thrown when a function fails, though not due to error.
   *  A "failure" is distinct from an "error" as an failure is typically
   *  nonfatal. For example, a client may not be able to join because the
   *  server is full. This is not exactly an error condition.
   */
  class Failure : public Error
  {
    using Error::Error;
  };
}
#endif

