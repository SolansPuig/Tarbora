#pragma once
#include <utility>
#include "Window.hpp"
#include "KeyCodes.hpp"

namespace Tarbora {
    class GraphicView;

    class InputImpl {
    public:
        enum State { UNCHANGED, UP, DOWN};

        InputImpl(GraphicView *m_View);
        ~InputImpl();

        bool GetKey(int keycode);
        bool GetKeyDown(int keycode);
        bool GetKeyUp(int keycode);
        State GetKeyState(int keycode);

        void SetKeyState(int keycode, State state) { m_key_state[keycode] = state; }

        bool GetButton(int button);
        bool GetButtonDown(int button);
        bool GetButtonUp(int button);
        State GetButtonState(int button);

        void SetButtonState(int button, State state) { m_button_state[button] = state; }

    private:
        GLFWwindow *m_window;
        GraphicView *m_View;

        std::vector<State> m_key_state;
        std::vector<State> m_button_state;

        std::pair<int, int> GetMousePosition();
    };
}
