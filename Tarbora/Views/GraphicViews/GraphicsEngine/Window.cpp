#include "GraphicsEngine.hpp"
#include "../../../Framework/Module.hpp"
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../Framework/External/stb_image_write.h"
#include "../../../Framework/External/tracy/TracyOpenGL.hpp"

namespace Tarbora {
    Window::Window(const char *title, int width, int height, GraphicsEngine *graphicsEngine) :
        m_Props(WindowProps(title, width, height, graphicsEngine))
    {
        LOG_DEBUG("Creating window with title: %s, width: %d and height: %d", m_Props.title, m_Props.width, m_Props.height);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.title, NULL, NULL);
        if (m_Window == NULL) {
            LOG_ERR("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, &m_Props);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
            data.ratio = (float)width / (float)height;

            glViewport(0, 0, width, height);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.graphicsEngine->GetModule()->Close();
        });

        stbi_flip_vertically_on_write(1);
    }

    Window::~Window()
    {
        if (m_Window)
            glfwDestroyWindow(m_Window);
    }

    void Window::Close()
    {
        if (m_Window)
            glfwDestroyWindow(m_Window);
        else
            LOG_ERR("Trying to close an unexisting GLFW window");
    }

    void Window::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window::CaptureMouse(bool capture)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, (capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    }

    void Window::SetTitle(const char* title)
    {
        glfwSetWindowTitle(m_Window, title);
    }

    int Window::TakeScreenshot(const std::string &filename)
    {
        // Get the window width and height and reserve memory
        int width = m_Props.width;
        int height = m_Props.height;
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

        LOG_INFO("Window: Saved screenshot %s", (filename + buffer).c_str());
        return saved;
    }
}
