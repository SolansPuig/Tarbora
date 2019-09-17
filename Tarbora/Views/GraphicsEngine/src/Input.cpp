#include "../inc/Input.hpp"
#include "../../GraphicViews/inc/GraphicView.hpp"

namespace Tarbora {
    InputImpl::InputImpl(GraphicView *view) :
        m_View(view),
        m_key_state(KEY_LAST, State::UNCHANGED),
        m_button_state(MOUSE_BUTTON_LAST, State::UNCHANGED)
    {
        m_window = m_View->GraphicsEngine()->GetWindow()->GetRawWindow();

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.view->Input()->SetKeyState(key, State::UP);
                    break;
                case GLFW_PRESS:
                    data.view->Input()->SetKeyState(key, State::DOWN);
                    break;
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.view->Input()->SetButtonState(button, InputImpl::State::UP);
                    break;
                case GLFW_PRESS:
                    data.view->Input()->SetButtonState(button, InputImpl::State::DOWN);
                    break;
            }
        });
    }

    InputImpl::~InputImpl()
    {

    }

    bool InputImpl::GetKey(int keycode)
    {
        auto state = glfwGetKey(m_window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputImpl::GetKeyDown(int keycode)
    {
        InputImpl::State state = m_key_state[keycode];
        if (state == InputImpl::State::DOWN)
        {
            m_key_state[keycode] = InputImpl::State::UNCHANGED;
            return true;
        }
        return false;
    }

    bool InputImpl::GetKeyUp(int keycode)
    {
        InputImpl::State state = m_key_state[keycode];
        if (state == InputImpl::State::UP)
        {
            m_key_state[keycode] = InputImpl::State::UNCHANGED;
            return true;
        }
        return false;
    }

    InputImpl::State InputImpl::GetKeyState(int keycode)
    {
        InputImpl::State state = m_key_state[keycode];
        m_key_state[keycode] = InputImpl::State::UNCHANGED;
        return state;
    }

    bool InputImpl::GetButton(int button)
    {
        auto state = glfwGetMouseButton(m_window, button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputImpl::GetButtonDown(int button)
    {
        InputImpl::State state = m_button_state[button];
        if (state == InputImpl::State::UP)
        {
            m_button_state[button] = InputImpl::State::UNCHANGED;
            return true;
        }
        return false;
    }

    bool InputImpl::GetButtonUp(int button)
    {
        InputImpl::State state = m_button_state[button];
        if (state == InputImpl::State::UP)
        {
            m_button_state[button] = InputImpl::State::UNCHANGED;
            return true;
        }
        return false;
    }

    InputImpl::State InputImpl::GetButtonState(int button)
    {
        InputImpl::State state = m_button_state[button];
        m_button_state[button] = InputImpl::State::UNCHANGED;
        return state;
    }

    std::pair<int, int> InputImpl::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        return { (int)xpos, (int)ypos };
    }
}
