/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#ifndef __WINDOW_H_
#define __WINDOW_H_

#include "../../../Framework/Global.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
// #include <GL/glext.h>
#include "External/glfw/include/GLFW/glfw3.h"
#define GLM_FORCE_CXX98

namespace Tarbora {
  class GraphicsEngine;

  struct WindowProps {
    const std::string title;
    int width;
    int height;
    float ratio;
    GraphicsEngine *graphics_engine;

    WindowProps(
      const std::string &title,
      int width, int height,
      GraphicsEngine *graphics_engine
    ) :
      title(title), width(width), height(height), ratio((float)width/(float)height),
      graphics_engine(graphics_engine) {};
  };

  class Window
  {
  public:
    Window(
      const std::string &title,
      int width, int height,
      GraphicsEngine *graphics_engine
    );

    ~Window();

    void setTitle(const std::string &title);

    void close();
    void update();

    void captureMouse(bool capture);

    int getWidth() { return props_.width; }
    int getHeight() { return props_.height; }
    float getRatio() { return props_.ratio; }

    GLFWwindow* getRawWindow() { return window_; }

    int takeScreenshot(const std::string &filename);
  private:
    GLFWwindow* window_;
    WindowProps props_;
  };
}

#endif // __WINDOW_H_
