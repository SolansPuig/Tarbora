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

        bool GetKey(int keycode);
        bool GetKeyDown(int keycode);
        bool GetKeyUp(int keycode);
        State GetKeyState(int keycode);

        void SetKeyState(int keycode, State state) { m_KeyState[keycode] = state; }

        bool GetButton(int button);
        bool GetButtonDown(int button);
        bool GetButtonUp(int button);
        State GetButtonState(int button);

        void SetButtonState(int button, State state) { m_ButtonState[button] = state; }

        glm::vec2 GetMousePosition();
        glm::vec2 GetMouseDelta();

    private:
        std::shared_ptr<Window> m_Window;

        std::vector<State> m_KeyState;
        std::vector<State> m_ButtonState;
        glm::vec2 m_LastMousePosition;
    };
}
