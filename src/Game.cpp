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
 * File:    Game.cpp
 * ----------------------------------------------------------------------------
 */
#include "Game.h"
namespace wic
{
  const unsigned CONTINUE = 1;
  const unsigned TERMINATE = 2;
  static bool focus = false;
  static bool downKeys[360] = {0};
  static bool pressedKeys[360] = {0};
  static string input;
  static Pair cursorLocation;
  static Pair scrollOffset;
  void resetInput()
  {
    memset(pressedKeys, 0, sizeof(pressedKeys));
    input.clear();
    scrollOffset = Pair();
  }
  void errorCallback(int error, const char* description)
  {
    throw WicError(WIC_ERRNO_GLFW_FAIL);
  }
  void focusCallback(GLFWwindow* window, int n)
  {
    focus = (bool) n;
  }
  void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                  int mods)
  {
    if(focus && key < 360)
    {
      if(action == GLFW_RELEASE)
        downKeys[key] = false;
      else if(action == GLFW_PRESS)
      {
        downKeys[key] = true;
        pressedKeys[key] = true;
      }
    }
  }
  void charCallback(GLFWwindow* window, unsigned int key)
  {
    if(focus)
      input.append(string((char*) &key));
  }
  void cursorLocationCallback(GLFWwindow* window, double x, double y)
  {
    if(focus)
    {
      cursorLocation.x = x;
      cursorLocation.y = y;
    }
  }
  void mouseButtonCallback(GLFWwindow* window, int button, int action,
                           int mods)
  {
    if(focus)
    {
      if(action == GLFW_PRESS)
        downKeys[button + 350] = true;
      else
        downKeys[button + 350] = false;
    }
  }
  void scrollCallback(GLFWwindow* window, double x, double y)
  {
    if(focus)
    {
      scrollOffset.x = x;
      scrollOffset.y = y;
    }
  }
  static bool initialized = false;
  Game::Game(string title, Pair dimensions, unsigned fps,
             bool resizeable, bool fullscreen, unsigned samples)
  {
    if(initialized)
      throw WicError(WIC_ERRNO_ALREADY_INIT);
    if(dimensions.x < 32)
      throw WicError(WIC_ERRNO_SMALL_X_DIMENSION);
    if(dimensions.y < 32)
      throw WicError(WIC_ERRNO_SMALL_Y_DIMENSION);
    if(!fps)
      throw WicError(WIC_ERRNO_SMALL_FPS);
    if(!glfwInit())
      throw WicError(WIC_ERRNO_GLFW_FAIL);
    glfwWindowHint(GLFW_REFRESH_RATE, fps);
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(!monitor)
    {
      glfwTerminate();
      throw WicError(WIC_ERRNO_MONITOR_FAIL);
    }
    if(fullscreen)
      window_ = glfwCreateWindow(dimensions.x, dimensions.y, title.data(), monitor,
                                0);
    else
      window_ = glfwCreateWindow(dimensions.x, dimensions.y, title.data(), 0, 0);
    if(!window_)
    {
      glfwTerminate();
      throw WicError(WIC_ERRNO_WINDOW_FAIL);
    }
    glfwSetErrorCallback(errorCallback);
    glfwSetWindowFocusCallback(window_, focusCallback);
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetCharCallback(window_, charCallback);
    glfwSetCursorPosCallback(window_, cursorLocationCallback);
    glfwSetScrollCallback(window_, scrollCallback);
    glfwMakeContextCurrent(window_);
    glfwSetTime(0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    FT_Library FTLibrary_;
    int error = FT_Init_FreeType(&FTLibrary_);
    if(error != 0)
    {
      glfwTerminate();
      glfwDestroyWindow(window_);
      throw WicError(WIC_ERRNO_FREETYPE_FAIL);
    }
    int physicalWidth; int physicalHeight;
    glfwGetMonitorPhysicalSize(monitor, &physicalWidth, &physicalHeight);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    pixelDensity_ = Pair(mode->width / (physicalWidth * 0.0393701),
                         mode->height / (physicalHeight * 0.0393701));
    secondsPerFrame_ = 1.0 / fps;
    previousTime_ = 0.0;
    delta_ = 0.0;
    initialized = true;
  }
  Game::~Game()
  {
    glfwDestroyWindow(window_);
    glfwTerminate();
    
    window_ = 0;
    dimensions_ = (Pair) {0,0};
    secondsPerFrame_ = 0.0;
    previousTime_ = 0.0;
    pixelDensity_ = (Pair) {0,0};
    initialized = false;
  }
  unsigned Game::updt()
  {
    if(!glfwWindowShouldClose(window_))
    {
      float delay = secondsPerFrame_ - (glfwGetTime() - previousTime_);
      if(delay > 0)
        usleep(delay * 1000);
      resetInput();
      glfwSwapBuffers(window_);
      glFlush();
      glClearColor(0.0,0.0,0.0,1.0);
      glClear(GL_COLOR_BUFFER_BIT);
      glLoadIdentity();
      delta_ = glfwGetTime() - previousTime_;
      previousTime_ = glfwGetTime();
      glfwPollEvents();
      return CONTINUE;
    }
    return TERMINATE;
  }
  bool Game::exit()
  {
    glfwSetWindowShouldClose(window_, true);
    return true;
  }
  double Game::getDelta()
  {
    return delta_;
  }
  bool Game::isKeyDown(enum Key key)
  {
    return downKeys[(int) key];
  }
  bool Game::isKeyPressed(enum Key key)
  {
    return pressedKeys[(int) key];
  }
  string Game::getInput()
  {
    return input;
  }
  Pair Game::getCursorLocation(Game* game)
  {
    Pair result = cursorLocation;
    result.y = game->dimensions_.y - result.y;
    return result;
  }
  Pair Game::getScrollOffset()
  {
    return scrollOffset;
  }
  double Game::getTime()
  {
    return glfwGetTime();
  }
  Pair Game::getDimensions()
  {
    return dimensions_;
  }

  Pair convertLocation(Pair location, Pair dimensions)
  {
    Pair result = location * Pair(2,2);
    result = result / dimensions;
    return result - Pair(1,1);
  }
}
