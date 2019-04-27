#include "../inc/GraphicsEngine.hpp"
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../Utility/inc/stb_image_write.h"
#include "../../ResourceManager/inc/Json.hpp"

namespace Tarbora {
    namespace GraphicsEngine {
        std::shared_ptr<Window> Main_Window;
        std::unique_ptr<Gui> m_Gui;
        std::shared_ptr<Shader> m_Shader;

        void Init()
        {
            glfwInit();

            std::string window_title = "Tarbora Game Engine";
            int window_width = 1280, window_height = 720;

            JsonPtr settings = GET_RESOURCE(Json, "Settings.json");
            if (settings)
            {
                json window;
                settings->Get("window", &window, {true});
                if (!window.empty())
                {
                    settings->PushErrName("window");

                    settings->Get(window, "title", &window_title, {true});

                    settings->GetArray(window, "size", 0, &window_width, {true});
                    settings->GetArray(window, "size", 1, &window_height, {true});

                    settings->PopErrName();
                }
            };

            Main_Window = std::unique_ptr<Window>(new Window(window_title.c_str(), window_width, window_height));

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) {
                LOG_ERR("GraphicsEngine: Failed to initialize GLEW");
                glfwTerminate();
            }

            m_Gui = std::unique_ptr<Gui>(new Gui());

            stbi_flip_vertically_on_write(1);

            ResourceManager::RegisterLoader(LoaderPtr(new ShaderResourceLoader()));
            ResourceManager::RegisterLoader(LoaderPtr(new TextureResourceLoader()));
            ResourceManager::RegisterLoader(LoaderPtr(new MeshResourceLoader()));
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

        void DrawMesh(std::shared_ptr<Mesh> mesh)
        {
            glBindVertexArray(mesh->GetId());
            glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertices());
        }

        void AfterDraw()
        {
            m_Gui->AfterDraw();
            Main_Window->Update();
        }

        void BeforeDrawSky()
        {
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);
        }

        void AfterDrawSky()
        {
            glDepthFunc(GL_LESS);
            glEnable(GL_CULL_FACE);
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
                LOG_ERR("ShaderCompiler: Error while compiling %s shader. \n %s", type.c_str(), infoLog);
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

        unsigned int LoadTexture(unsigned char *data, int width, int height, int nrComponents)
        {
            // Detect the format
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else
                format = GL_RGBA;

            // Create the texture
            unsigned int id = 0;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Configure the texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            return id;
        }

        void BindTexture(unsigned int id)
        {
            glBindTexture(GL_TEXTURE_2D, id);
        }

        void DeleteTexture(unsigned int id)
        {
            glDeleteTextures(1, &id);
        }

        unsigned int LoadMesh(std::vector<float> vertices)
        {
            unsigned int VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

            glBindVertexArray(VAO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // glDeleteBuffers(1, &VBO);
            return VAO;
        }

        void DeleteMesh(unsigned int id)
        {
            glDeleteVertexArrays(1, &id);
        }

        void UseShader(std::shared_ptr<Shader> shader)
        {
            m_Shader = shader;
            m_Shader->Use();
        }

        std::shared_ptr<Shader> GetShader()
        {
            return m_Shader;
        }

        bool ShaderAvailable()
        {
            return m_Shader != nullptr;
        }

        int TakeScreenshot(const std::string &filename)
        {
            // Get the window width and height and reserve memory
            int width = Main_Window->GetWidth();
            int height = Main_Window->GetHeight();
            char *data = (char*) malloc((size_t) (width * height * 3));

            // Configure the format for storing the pixels and read them
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, width, height,  GL_RGB, GL_UNSIGNED_BYTE, data);

            // Generate the timestamp for the name
            time_t t = time(NULL);
            char buffer[30];
            strftime(buffer, 30, "_%Y%m%d_%H%M%S.png", localtime(&t));

            // Store the screnshoot and free the reserved memory
            int saved = stbi_write_png((filename + buffer).c_str(), width, height, 3, data, 0);
            free(data);

            LOG_INFO("GraphicsEngine: Saved screenshot %s", (filename + buffer).c_str());
            return saved;
        }
    }
}
