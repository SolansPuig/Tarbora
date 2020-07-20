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

#ifndef __INPUT_H_
#define __INPUT_H_

#include <utility>
#include "Window.hpp"
#include "KeyCodes.hpp"

namespace Tarbora {
  class GraphicsEngine;

  class Input {
  public:
    enum State { UNCHANGED, UP, DOWN};

    Input(GraphicsEngine *graphicsEngine);
    ~Input();

    bool getKey(int keycode);
    bool getKeyDown(int keycode);
    bool getKeyUp(int keycode);
    State getKeyState(int keycode);

    void setKeyState(int keycode, State state) { key_state_[keycode] = state; }

    bool getButton(int button);
    bool getButtonDown(int button);
    bool getButtonUp(int button);
    State getButtonState(int button);

    void setButtonState(int button, State state) { button_state_[button] = state; }

    void captureMouse(bool capture);
    void toggleCaptureMouse();
    glm::vec2 forceGetMousePosition();
    glm::vec2 forceGetMouseDelta();
    glm::vec2 getMousePosition();
    glm::vec2 getMouseDelta();

    void setScroll(float scroll);
    float getScroll();
    float getScrollDelta();
    bool mouseScrolled();

    void enableInput(bool enabled) { enabled_ = enabled; }
    bool enabled() { return enabled_; }

  private:
    std::shared_ptr<Window> window_;

    std::vector<State> key_state_;
    std::vector<State> button_state_;
    glm::vec2 last_mouse_position_{glm::vec2(0.f, 0.f)};
    bool mouse_scrolled_{false};
    float mouse_scroll_{0.f};
    float mouse_scroll_delta_{0.f};
    bool capture_mouse_{true};

    bool enabled_{true};
  };
}

#endif // __INPUT_H_
