#include "Input.hpp"
#include "Graphics_Engine.hpp"
#include "Logger.hpp"

namespace Tarbora {
    namespace Input {
        GLFWwindow *window;

        void Init()
        {
            window = Graphics_Engine::GetWindow()->GetWindow();
        }

        bool IsKeyPressed(int keycode)
        {
            auto state = glfwGetKey(window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool IsMouseButtonPressed(int button)
        {
            return glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        std::pair<int, int> GetMousePosition()
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return { (int)xpos, (int)ypos };
        }

    }
}