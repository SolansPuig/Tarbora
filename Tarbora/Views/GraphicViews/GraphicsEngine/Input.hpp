#pragma once
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

        glm::vec2 getMousePosition();
        glm::vec2 getMouseDelta();

        void setScroll(float scroll);
        float getScroll();
        float getScrollDelta();
        bool mouseScrolled();

    private:
        std::shared_ptr<Window> window_;

        std::vector<State> key_state_;
        std::vector<State> button_state_;
        glm::vec2 last_mouse_position_{glm::vec2(0.f, 0.f)};
        bool mouse_scrolled_{false};
        float mouse_scroll_{0.f};
        float mouse_scroll_delta_{0.f};
    };
}
