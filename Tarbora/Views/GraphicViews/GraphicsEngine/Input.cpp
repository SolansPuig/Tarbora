#include "Input.hpp"
#include "GraphicsEngine.hpp"

namespace Tarbora {
    Input::Input(GraphicsEngine *graphicsEngine) :
        m_KeyState(KEY_LAST, State::UNCHANGED),
        m_ButtonState(MOUSE_BUTTON_LAST, State::UNCHANGED),
        m_LastMousePosition(glm::vec2(0.f, 0.f))
    {
        m_Window = graphicsEngine->GetWindow();

        glfwSetKeyCallback(m_Window->GetRawWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.graphicsEngine->GetInputManager()->SetKeyState(key, State::UP);
                    break;
                case GLFW_PRESS:
                    data.graphicsEngine->GetInputManager()->SetKeyState(key, State::DOWN);
                    break;
            }
        });

        glfwSetMouseButtonCallback(m_Window->GetRawWindow(), [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.graphicsEngine->GetInputManager()->SetButtonState(button, State::UP);
                    break;
                case GLFW_PRESS:
                    data.graphicsEngine->GetInputManager()->SetButtonState(button, State::DOWN);
                    break;
            }
        });
    }

    Input::~Input()
    {

    }

    bool Input::GetKey(int keycode)
    {
        auto state = glfwGetKey(m_Window->GetRawWindow(), keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::GetKeyDown(int keycode)
    {
        State state = m_KeyState[keycode];
        if (state == State::DOWN)
        {
            m_KeyState[keycode] = State::UNCHANGED;
            return true;
        }
        return false;
    }

    bool Input::GetKeyUp(int keycode)
    {
        State state = m_KeyState[keycode];
        if (state == State::UP)
        {
            m_KeyState[keycode] = State::UNCHANGED;
            return true;
        }
        return false;
    }

    Input::State Input::GetKeyState(int keycode)
    {
        State state = m_KeyState[keycode];
        m_KeyState[keycode] = State::UNCHANGED;
        return state;
    }

    bool Input::GetButton(int button)
    {
        auto state = glfwGetMouseButton(m_Window->GetRawWindow(), button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::GetButtonDown(int button)
    {
        State state = m_ButtonState[button];
        if (state == State::DOWN)
        {
            m_ButtonState[button] = State::UNCHANGED;
            return true;
        }
        return false;
    }

    bool Input::GetButtonUp(int button)
    {
        State state = m_ButtonState[button];
        if (state == State::UP)
        {
            m_ButtonState[button] = State::UNCHANGED;
            return true;
        }
        return false;
    }

    Input::State Input::GetButtonState(int button)
    {
        State state = m_ButtonState[button];
        m_ButtonState[button] = State::UNCHANGED;
        return state;
    }

    glm::vec2 Input::GetMouseDelta()
    {
        glm::vec2 mousePosition = GetMousePosition();
        glm::vec2 delta =  m_LastMousePosition - mousePosition;
        m_LastMousePosition = mousePosition;
        return delta;
    }

    glm::vec2 Input::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(m_Window->GetRawWindow(), &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }
}
