/* ----------------------------------------------------------------------------
 * wic - a simple 2D game engine for Mac OSX written in C++
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
 * File:    Error.cpp
 * ----------------------------------------------------------------------------
 */
#include "Error.h"
namespace wic
{
  WicError::WicError(ErrorCode code)
  {
    message_ = "[WIC ERROR]: ";
    switch(code)
    {
      case WIC_ERRNO_NONE:
        message_.append("no error"); break;
      case WIC_ERRNO_ALREADY_INIT:
        message_.append("an instance is already initialized"); break;
      case WIC_ERRNO_NO_HEAP:
        message_.append("out of heap memory"); break;
      case WIC_ERRNO_LOAD_FILE_FAIL:
        message_.append("file could not be found or loaded"); break;
      case WIC_ERRNO_NULL_TARGET:
        message_.append("target is null"); break;
      case WIC_ERRNO_NULL_GAME:
        message_.append("game is null"); break;
      case WIC_ERRNO_SMALL_X_DIMENSION:
        message_.append("dimensions.x is too small"); break;
      case WIC_ERRNO_SMALL_Y_DIMENSION:
        message_.append("dimensions.y is too small"); break;
      case WIC_ERRNO_NULL_TITLE:
        message_.append("title is null"); break;
      case WIC_ERRNO_SMALL_TITLE:
        message_.append("title has zero characters"); break;
      case WIC_ERRNO_SMALL_FPS:
        message_.append("fps is less than 1"); break;
      case WIC_ERRNO_GLFW_FAIL:
        message_.append("GLFW: initialization failed"); break;
      case WIC_ERRNO_MONITOR_FAIL:
        message_.append("GLFW: monitor fetch failed"); break;
      case WIC_ERRNO_WINDOW_FAIL:
        message_.append("GLFW: window creation failed"); break;
      case WIC_ERRNO_FREETYPE_FAIL:
        message_.append("Freetype: initialization failed"); break;
      case  WIC_ERRNO_NULL_BUFFER:
        message_.append("buffer is null"); break;
      case WIC_ERRNO_NO_GPU_MEM:
        message_.append("out of GPU memory"); break;
      case WIC_ERRNO_NULL_FILEPATH:
        message_.append("filepath is null"); break;
      case WIC_ERRNO_NULL_TEXTURE:
        message_.append("texture is null"); break;
      case WIC_ERRNO_NULL_VERTICES:
        message_.append("vertices is null"); break;
      case WIC_ERRNO_SMALL_NUM_VERTICES:
        message_.append("num_vertices is less than 3"); break;
      case WIC_ERRNO_SMALL_POINT:
        message_.append("point is 0"); break;
      case WIC_ERRNO_NULL_RESULT:
        message_.append("result is null"); break;
      case WIC_ERRNO_NULL_FONT:
        message_.append("font is null"); break;
      case WIC_ERRNO_NULL_STRING:
        message_.append("string is null"); break;
      case WIC_ERRNO_INVALID_RED:
        message_.append("red is not in range 0-255"); break;
      case WIC_ERRNO_INVALID_GREEN:
        message_.append("green is not in range 0-255"); break;
      case WIC_ERRNO_INVALID_BLUE:
        message_.append("blue is not in range 0-255"); break;
      case WIC_ERRNO_INVALID_ALPHA:
        message_.append("alpha is not in range 0-255"); break;
      case WIC_ERRNO_NULL_PACKET:
        message_.append("packet is null"); break;
      case WIC_ERRNO_NULL_NAME:
        message_.append("name is null"); break;
      case WIC_ERRNO_RESERVED_PORT:
        message_.append("port is reserved"); break;
      case WIC_ERRNO_SMALL_NAME:
        message_.append("name has 0 characters"); break;
      case WIC_ERRNO_LARGE_NAME:
        message_.append("name has > 20 characters"); break;
      case WIC_ERRNO_NULL_SERVER_IP:
        message_.append("server_ip is null"); break;
      case WIC_ERRNO_SMALL_LEN_SERVER_IP:
        message_.append("len_server_ip is 0"); break;
      case WIC_ERRNO_SOCKET_FAIL:
        message_.append("socket creation failed"); break;
      case WIC_ERRNO_PORT_IN_USE:
        message_.append("port in use"); break;
      case WIC_ERRNO_SOCKET_BIND_FAIL:
        message_.append("socket binding failed"); break;
      case WIC_ERRNO_CLIENT_ALREADY_JOINED:
        message_.append("client is already joined");
      case WIC_ERRNO_JOIN_FAIL_FULL:
        message_.append("server is full"); break;
      case WIC_ERRNO_JOIN_FAIL_BANNED:
        message_.append("client is banned"); break;
      case WIC_ERRNO_CLIENT_NOT_JOINED:
        message_.append("client not joined"); break;
      case WIC_ERRNO_TIMEOUT:
        message_.append("operation timed out"); break;
      case WIC_ERRNO_PACKET_UNKNOWN_SOURCE:
        message_.append("packet from unknown source"); break;
      case WIC_ERRNO_SMALL_MAX_CLIENTS:
        message_.append("max_clients < 1"); break;
      case WIC_ERRNO_LARGE_MAX_CLIENTS:
        message_.append("max_clients > 254"); break;
      case WIC_ERRNO_NOT_CLIENT_INDEX:
        message_.append("index < 1 (not a client index)"); break;
      case WIC_ERRNO_IMPOSSIBLE_INDEX:
        message_.append("index impossible given max clients"); break;
      case WIC_ERRNO_INDEX_UNUSED:
        message_.append("index not in use"); break;
      case WIC_ERRNO_LARGE_REASON:
        message_.append("reason has > 50 characters"); break;
      case WIC_ERRNO_NULL_NAME_OR_IP:
        message_.append("name_or_ip is null");
      case WIC_ERRNO_LARGE_NAME_OR_IP:
        message_.append("name_or_ip has > 20 characters"); break;
      case WIC_ERRNO_UNBANNED_NAME_OR_IP:
        message_.append("name_or_ip was never banned"); break;
      case WIC_ERRNO_NO_SUCH_CLIENT:
        message_.append("no client found for name_or_ip"); break;
    }
    message_.append("\n");
  }
  string WicError::what() { return message_; };
}

