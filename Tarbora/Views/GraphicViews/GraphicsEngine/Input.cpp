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

#include "Input.hpp"
#include "GraphicsEngine.hpp"

namespace Tarbora {
  Input::Input(GraphicsEngine *graphicsEngine) :
    key_state_(KEY_LAST, State::UNCHANGED),
    button_state_(MOUSE_BUTTON_LAST, State::UNCHANGED)
  {
    window_ = graphicsEngine->getWindow();

    glfwSetKeyCallback(
      window_->getRawWindow(),
      [](GLFWwindow* window, int key, int, int action, int)
      {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

        if (!data.graphics_engine->getInputManager()->enabled()) {UNUSED(data); return;}

        switch (action) {
          case GLFW_RELEASE:
            data.graphics_engine->getInputManager()->setKeyState(key, State::UP);
            break;
          case GLFW_PRESS:
            data.graphics_engine->getInputManager()->setKeyState(key, State::DOWN);
            break;
        }
      });

    glfwSetMouseButtonCallback(
      window_->getRawWindow(),
      [](GLFWwindow* window, int button, int action, int)
      {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

        if (!data.graphics_engine->getInputManager()->enabled()) {UNUSED(data); return;}

        switch (action) {
          case GLFW_RELEASE:
            data.graphics_engine->getInputManager()->setButtonState(button, State::UP);
            break;
          case GLFW_PRESS:
            data.graphics_engine->getInputManager()->setButtonState(button, State::DOWN);
            break;
        }
      });

    glfwSetScrollCallback(
      window_->getRawWindow(),
      [](GLFWwindow* window, double, double yoffset)
      {
        WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

        if (!data.graphics_engine->getInputManager()->enabled()) {UNUSED(data); return;}

        data.graphics_engine->getInputManager()->setScroll(yoffset);
      });
  }

  Input::~Input()
  {

  }

  bool Input::getKey(int keycode)
  {
    if (!enabled_) return false;

    auto state = glfwGetKey(window_->getRawWindow(), keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::getKeyDown(int keycode)
  {
    if (!enabled_) return false;

    State state = key_state_[keycode];
    if (state == State::DOWN)
    {
      key_state_[keycode] = State::UNCHANGED;
      return true;
    }
    return false;
  }

  bool Input::getKeyUp(int keycode)
  {
    if (!enabled_) return false;

    State state = key_state_[keycode];
    if (state == State::UP)
    {
      key_state_[keycode] = State::UNCHANGED;
      return true;
    }
    return false;
  }

  Input::State Input::getKeyState(int keycode)
  {
    State state = key_state_[keycode];
    key_state_[keycode] = State::UNCHANGED;
    return state;
  }

  bool Input::getButton(int button)
  {
    if (!enabled_) return false;

    auto state = glfwGetMouseButton(window_->getRawWindow(), button);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::getButtonDown(int button)
  {
    if (!enabled_) return false;

    State state = button_state_[button];
    if (state == State::DOWN)
    {
      button_state_[button] = State::UNCHANGED;
      return true;
    }
    return false;
  }

  bool Input::getButtonUp(int button)
  {
    if (!enabled_) return false;

    State state = button_state_[button];
    if (state == State::UP)
    {
      button_state_[button] = State::UNCHANGED;
      return true;
    }
    return false;
  }

  Input::State Input::getButtonState(int button)
  {
    State state = button_state_[button];
    button_state_[button] = State::UNCHANGED;
    return state;
  }

  void Input::captureMouse(bool capture)
  {
    capture_mouse_ = capture;
    window_->captureMouse(capture);
    last_mouse_position_ = getMousePosition();
  }

  glm::vec2 Input::getMouseDelta()
  {
    glm::vec2 mouse_position = getMousePosition();
    glm::vec2 delta =  last_mouse_position_ - mouse_position;
    last_mouse_position_ = mouse_position;
    return delta;
  }

  glm::vec2 Input::getMousePosition()
  {
    double xpos, ypos;
    glfwGetCursorPos(window_->getRawWindow(), &xpos, &ypos);
    return glm::vec2(xpos, ypos);
  }

  void Input::setScroll(float scroll)
  {
    mouse_scrolled_ = true;
    mouse_scroll_ += scroll;
    mouse_scroll_delta_ = scroll;
  }

  float Input::getScroll()
  {
    mouse_scrolled_ = false;
    return mouse_scroll_;
  }

  float Input::getScrollDelta()
  {
    mouse_scrolled_ = false;
    return mouse_scroll_delta_;
  }

  bool Input::mouseScrolled()
  {
    if (!enabled_) return false;

    bool r = mouse_scrolled_;
    mouse_scrolled_ = false;
    return r;
  }
}
