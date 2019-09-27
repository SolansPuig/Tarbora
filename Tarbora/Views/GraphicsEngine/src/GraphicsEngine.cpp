#include "../inc/GraphicsEngine.hpp"
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../Framework/External/stb_image_write.h"
#include "../../../Framework/ResourceManager/inc/Json.hpp"

namespace Tarbora {
    GraphicsEngineImpl::GraphicsEngineImpl(GraphicView *view, std::string settingsFile) :
        m_View(view)
    {
        glfwInit();

        std::string windowTitle = "Tarbora Game Engine";
        int windowWidth = 1280, windowHeight = 720;

        JsonPtr settings = GET_RESOURCE(Json, settingsFile);
        if (settings)
        {
            raw_json window;
            settings->Get("window", &window, {true});
            if (!window.empty())
            {
                settings->PushErrName("window");

                settings->Get(window, "title", &windowTitle, {true});

                settings->GetArray(window, "size", 0, &windowWidth, {true});
                settings->GetArray(window, "size", 1, &windowHeight, {true});

                settings->PopErrName();
            }
        };

        m_Window = std::unique_ptr<Window>(new Window(windowTitle.c_str(), windowWidth, windowHeight, m_View));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glViewport(0, 0, windowWidth, windowHeight);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            LOG_ERR("GraphicsEngineImpl: Failed to initialize GLEW");
            glfwTerminate();
        }

        stbi_flip_vertically_on_write(1);

        ResourceManager::RegisterLoader(LoaderPtr(new ShaderResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new TextureResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new MeshResourceLoader()));

        GenerateFramebuffer(windowWidth, windowHeight);
    }

    GraphicsEngineImpl::~GraphicsEngineImpl()
    {
        LOG_DEBUG("Destroying Graphics Engine");
        glfwTerminate();
    }

    void GraphicsEngineImpl::InitGui()
    {
        m_Gui = std::unique_ptr<Gui>(new Gui(m_View));
    }

    void GraphicsEngineImpl::BeforeDraw()
    {
        // Bind and clear the multisampling buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampledFBO);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        m_Gui->BeforeDraw();
    }

    void GraphicsEngineImpl::DrawMesh(std::shared_ptr<Mesh> mesh)
    {
        glBindVertexArray(mesh->GetId());
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertices());
    }

    void GraphicsEngineImpl::AfterDraw()
    {
        int width = m_Window->GetWidth();
        int height = m_Window->GetHeight();

        // Get the multisampling to the post processing buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultisampledFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_PostProcessFBO);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Apply post-processing
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        if (!m_ScreenShader.expired() && !m_QuadMesh.expired())
        m_ScreenShader.lock()->Use();
        glBindTexture(GL_TEXTURE_2D, m_TexturePostProcess);
        glBindVertexArray(m_QuadMesh.lock()->GetId());
        glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh.lock()->GetVertices());

        m_Gui->AfterDraw();
        m_Window->Update();
    }

    void GraphicsEngineImpl::BeforeDrawSky()
    {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
    }

    void GraphicsEngineImpl::AfterDrawSky()
    {
        glDepthFunc(GL_LESS);
    }

    WindowPtr GraphicsEngineImpl::GetWindow()
    {
        return m_Window;
    }

    unsigned int GraphicsEngineImpl::CompileShader(std::string type, const char *code)
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

    unsigned int GraphicsEngineImpl::LinkProgram(unsigned int *ids)
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

    void GraphicsEngineImpl::DeleteProgram(unsigned int id)
    {
        glDeleteProgram(id);
    }

    // Shader *GetShader()
    // {
    //     return &m_Shader;
    // }

    unsigned int GraphicsEngineImpl::LoadTexture(unsigned char *data, int width, int height, int nrComponents)
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

    void GraphicsEngineImpl::BindTexture(unsigned int id)
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void GraphicsEngineImpl::DeleteTexture(unsigned int id)
    {
        glDeleteTextures(1, &id);
    }

    unsigned int GraphicsEngineImpl::LoadMesh(std::vector<float> vertices)
    {
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));

        // glDeleteBuffers(1, &VBO);
        return VAO;
    }

    void GraphicsEngineImpl::DeleteMesh(unsigned int id)
    {
        glDeleteVertexArrays(1, &id);
    }

    void GraphicsEngineImpl::UseShader(std::shared_ptr<Shader> shader)
    {
        m_Shader = shader;
        m_Shader.lock()->Use();
    }

    std::shared_ptr<Shader> GraphicsEngineImpl::GetShader()
    {
        return m_Shader.lock();
    }

    bool GraphicsEngineImpl::ShaderAvailable()
    {
        return !m_Shader.expired();
    }

    void GraphicsEngineImpl::GenerateFramebuffer(int width, int height)
    {
        // Generate the multisampling buffer
        glGenFramebuffers(1, &m_MultisampledFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampledFBO);
        glGenTextures(1, &m_TextureMultisampled);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureMultisampled);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureMultisampled, 0);
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        // Generate the post processing buffer
        glGenFramebuffers(1, &m_PostProcessFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);

        glGenTextures(1, &m_TexturePostProcess);
        glBindTexture(GL_TEXTURE_2D, m_TexturePostProcess);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexturePostProcess, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_QuadMesh = GET_RESOURCE(Mesh, "meshes/plane.mesh");
        m_ScreenShader = GET_RESOURCE(Shader, "shaders/screen.shader.json");
    }

    int GraphicsEngineImpl::TakeScreenshot(const std::string &filename)
    {
        // Get the window width and height and reserve memory
        int width = m_Window->GetWidth();
        int height = m_Window->GetHeight();
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

        LOG_INFO("GraphicsEngineImpl: Saved screenshot %s", (filename + buffer).c_str());
        return saved;
    }
}
