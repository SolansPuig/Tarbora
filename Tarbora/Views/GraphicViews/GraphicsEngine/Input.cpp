#include "Input.hpp"
#include "GraphicsEngine.hpp"

namespace Tarbora {
    Input::Input(GraphicsEngine *graphicsEngine) :
        key_state_(KEY_LAST, State::UNCHANGED),
        button_state_(MOUSE_BUTTON_LAST, State::UNCHANGED),
        last_mouse_position_(glm::vec2(0.f, 0.f))
    {
        window_ = graphicsEngine->getWindow();

        glfwSetKeyCallback(window_->getRawWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            UNUSED(scancode); UNUSED(mods);

            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.graphics_engine->getInputManager()->setKeyState(key, State::UP);
                    break;
                case GLFW_PRESS:
                    data.graphics_engine->getInputManager()->setKeyState(key, State::DOWN);
                    break;
            }
        });

        glfwSetMouseButtonCallback(window_->getRawWindow(), [](GLFWwindow* window, int button, int action, int mods)
        {
            UNUSED(mods);

            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_RELEASE:
                    data.graphics_engine->getInputManager()->setButtonState(button, State::UP);
                    break;
                case GLFW_PRESS:
                    data.graphics_engine->getInputManager()->setButtonState(button, State::DOWN);
                    break;
            }
        });
    }

    Input::~Input()
    {

    }

    bool Input::getKey(int keycode)
    {
        auto state = glfwGetKey(window_->getRawWindow(), keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::getKeyDown(int keycode)
    {
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
        auto state = glfwGetMouseButton(window_->getRawWindow(), button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::getButtonDown(int button)
    {
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
}