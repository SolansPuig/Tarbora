#include "../inc/GraphicsEngine.hpp"
#include "../../GraphicViews/inc/GraphicView.hpp"

namespace Tarbora {
    Window::Window(const char *title, int width, int height, GraphicView *view) :
        m_Props(WindowProps(title, width, height, view))
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

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        glViewport(0, 0, m_Props.width, m_Props.height);

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
            data.view->Close();
        });
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

    void Window::Clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::CaptureMouse(bool capture)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, (capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    }

    void Window::SetTitle(const char* title)
    {
        glfwSetWindowTitle(m_Window, title);
    }
}
