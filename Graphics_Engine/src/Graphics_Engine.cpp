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

        unsigned int CompileShader(std::string type, const char *code)
        {
            // Create and compile the shader
            unsigned int id = 0;
            if (type == std::string("vertex")) id = glCreateShader(GL_VERTEX_SHADER);
            else if (type == std::string("tes_control")) id = glCreateShader(GL_TESS_CONTROL_SHADER);
            else if (type == std::string("tes_eval")) id = glCreateShader(GL_TESS_EVALUATION_SHADER);
            else if (type == std::string("geometry")) id = glCreateShader(GL_GEOMETRY_SHADER);
            else if (type == std::string("fragment")) id = glCreateShader(GL_FRAGMENT_SHADER);
            else if (type == std::string("compute")) id = glCreateShader(GL_COMPUTE_SHADER);
            else LOG_ERR("ShaderCompiler: Shader type %s not recognized", type.c_str());
            glShaderSource(id, 1, &code, NULL);
            glCompileShader(id);
            // Check for errors
            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(id, 512, NULL, infoLog);
                LOG_ERR("ShaderCompiler: Error while compiling %s shader. \n %s", type, infoLog);
            }

            return id;
        }

        unsigned int LinkProgram(unsigned int *ids)
        {
            // Attach all the shaders if they are valid and link the program
            unsigned int id = glCreateProgram();
            for (int i = 0; i < 6; i++)
                if (ids[i] != 0)
                    glAttachShader(id, ids[i]);
            glLinkProgram(id);

            // Check for errors
            GLint success;
            GLchar infoLog[512];
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(id, 512, NULL, infoLog);
                LOG_ERR("ShaderCompiler: Error while linking program. \n %s", infoLog);
            }

            // Delete all the shaders, as they are linked to the program and no longer needed
            for (int i = 0; i < 6; i++)
                glDeleteShader(ids[i]);

            return id;
        }

        void DeleteProgram(unsigned int id)
        {
            glDeleteProgram(id);
        }

        // Shader *GetShader()
        // {
        //     return &m_Shader;
        // }
    }
}
