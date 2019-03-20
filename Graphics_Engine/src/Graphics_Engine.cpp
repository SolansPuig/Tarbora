#include "Graphics_Engine.hpp"
#include <memory>
#include "Logger.hpp"
#include "Gui.hpp"
#include "Settings.hpp"

namespace Tarbora {
    namespace Graphics_Engine {
        std::shared_ptr<Window> Main_Window;
        std::unique_ptr<Gui> m_Gui;
        // Shader m_Shader;

        void Init()
        {
            glfwInit();
            Main_Window = std::unique_ptr<Window>(new Window(Settings::window_title.c_str(), Settings::window_width, Settings::window_height));

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) {
                LOG_ERR("Graphics_Engine: Failed to initialize GLEW");
                glfwTerminate();
            }

            // m_Shader.LoadFromFile("../resources/shaders/mainShader/vs", "../resources/shaders/mainShader/fs");
            m_Gui = std::unique_ptr<Gui>(new Gui());
        }

        void Close()
        {
            glfwTerminate();
        }

        void BeforeDraw()
        {
            Main_Window->Clear();
            m_Gui->BeforeDraw();
        }

        void AfterDraw()
        {
            m_Gui->AfterDraw();
            Main_Window->Update();
        }

        WindowPtr GetWindow()
        {
            return Main_Window;
        }

        // Shader *GetShader()
        // {
        //     return &m_Shader;
        // }
    }
}
