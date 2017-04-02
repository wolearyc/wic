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
    throw Error("glfw encountered an error");
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
  : dimensions(dimensions)
  {
    if(initialized)
      throw Error("a game is already initialized");
    if(dimensions.x < 32)
      throw InvalidArgument("dimensions.x", "< 32");
    if(dimensions.y < 32)
      throw InvalidArgument("dimensions.y", "< 32");
    if(fps == 0)
      throw InvalidArgument("fps", "zero");
    if(!glfwInit())
      throw InternalError("glfw failed to initialize");
    glfwWindowHint(GLFW_REFRESH_RATE, fps);
    glfwWindowHint(GLFW_SAMPLES, samples);
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(!monitor)
    {
      glfwTerminate();
      throw InternalError("glfw failed to fetch the primary monitor");
    }
    if(fullscreen)
      window = glfwCreateWindow(dimensions.x, dimensions.y, title.data(), monitor,
                                0);
    else
      window = glfwCreateWindow(dimensions.x, dimensions.y, title.data(), 0, 0);
    if(!window)
    {
      glfwTerminate();
      throw InternalError("window creation failed");
    }
    glfwSetErrorCallback(errorCallback);
    glfwSetWindowFocusCallback(window, focusCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetCursorPosCallback(window, cursorLocationCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwMakeContextCurrent(window);
    glfwSetTime(0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    FT_Library FTLibrary;
    int error = FT_Init_FreeType(&FTLibrary);
    if(error != 0)
    {
      glfwTerminate();
      glfwDestroyWindow(window);
      throw InternalError("freetype could not be initialized");
    }
    int physicalWidth; int physicalHeight;
    glfwGetMonitorPhysicalSize(monitor, &physicalWidth, &physicalHeight);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    pixelDensity = Pair(mode->width / (physicalWidth * 0.0393701),
                         mode->height / (physicalHeight * 0.0393701));
    secondsPerFrame = 1.0 / fps;
    previousTime = 0.0;
    delta = 0.0;
    initialized = true;
  }
  Game::~Game() 
  {
    glfwDestroyWindow(window);
    glfwTerminate();
    initialized = false;
  }
  unsigned Game::updt()
  {
    if(!glfwWindowShouldClose(window))
    {
      float delay = secondsPerFrame - (glfwGetTime() - previousTime);
      if(delay > 0)
        usleep(delay * 1000);
      resetInput();
      glfwSwapBuffers(window);
      glFlush();
      glClearColor(0.0,0.0,0.0,1.0);
      glClear(GL_COLOR_BUFFER_BIT);
      glLoadIdentity();
      delta = glfwGetTime() - previousTime;
      previousTime = glfwGetTime();
      glfwPollEvents();
      return CONTINUE;
    }
    return TERMINATE;
  }
  void Game::exit()
  {
    glfwSetWindowShouldClose(window, true);
  }
  double Game::getDelta() const
  {
    return delta;
  }
  bool Game::isKeyDown(enum Key key) const
  {
    return downKeys[(int) key];
  }
  bool Game::isKeyPressed(enum Key key) const
  {
    return pressedKeys[(int) key];
  }
  string Game::getInput() const
  {
    return input;
  }
  Pair Game::getCursorLocation() const
  {
    Pair result = cursorLocation;
    result.y = dimensions.y - result.y;
    return result;
  }
  Pair Game::getScrollOffset() const
  {
    return scrollOffset;
  }
  double Game::getTime() const
  {
    return glfwGetTime();
  }
  Pair Game::getDimensions() const
  {
    return dimensions;
  }
  Pair Game::getPixelDensity() const
  {
    return pixelDensity;
  }

  Pair convertLocation(Pair location, Pair dimensions)
  {
    Pair result = location * Pair(2,2);
    result = result / dimensions;
    return result - Pair(1,1);
  }
}
