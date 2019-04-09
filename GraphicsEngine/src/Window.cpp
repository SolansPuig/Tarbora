#include "Window.hpp"
#include "Logger.hpp"
#include "Events.hpp"
#include "Settings.hpp"

namespace Tarbora {
    Window::Window(const char *title, int width, int height)
        : m_Props(WindowProps(title, width, height))
    {
        LOG_DEBUG("Window: Creating window with title: %s, width: %d and height: %d", m_Props.m_title, m_Props.m_width, m_Props.m_height);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Props.m_width, m_Props.m_height, m_Props.m_title, NULL, NULL);
        if (m_Window == NULL) {
            LOG_ERR("Window: Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);

        glViewport(0, 0, m_Props.m_width, m_Props.m_height);

        glfwSetWindowUserPointer(m_Window, &m_Props);

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y)
        {
            (void)(window);
            WindowMoveEvent event(x, y);
            EventManager::Trigger(EventType::WindowMove, &event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
            (void)(window);
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.m_width = width;
			data.m_height = height;
            data.m_ratio = (float)width / (float)height;

            glViewport(0, 0, width, height);

			WindowResizeEvent event(width, height);
			EventManager::Trigger(EventType::WindowResize, &event);
        });

        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
        {
            (void)(window);
            WindowIconifyEvent event(iconified);
            EventManager::Trigger(EventType::WindowIconify, &event);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
        {
            (void)(window);
            WindowFocusEvent event(focused);
            EventManager::Trigger(EventType::WindowFocus, &event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
            (void)(window);
			WindowCloseEvent event;
			EventManager::Trigger(EventType::WindowClose, &event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            (void)(window); (void)(scancode);
            switch (action) {
                case GLFW_PRESS:
                {
                    KeyPressEvent event(key, mods, 0);
                    EventManager::Trigger(EventType::KeyPress, &event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent event(key, mods);
                    EventManager::Trigger(EventType::KeyRelease, &event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressEvent event(key, mods, 1);
                    EventManager::Trigger(EventType::KeyPress, &event);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
            (void)(window);
            MouseMoveEvent event((int)x, (int)y);
            EventManager::Trigger(EventType::MouseMove, &event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            (void)(window);
            switch (action) {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(button, mods);
                    EventManager::Trigger(EventType::MouseButtonPress, &event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent event(button, mods);
                    EventManager::Trigger(EventType::MouseButtonRelease, &event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
            (void)(window);
            MouseScrollEvent event((int)x, (int)y);
            EventManager::Trigger(EventType::MouseScroll, &event);
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
            LOG_ERR("Window: Trying to close an unexisting GLFW window");
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::Clear()
    {
        glClearColor(Settings::clear_r, Settings::clear_g, Settings::clear_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::SetTitle(const char* title)
    {
        glfwSetWindowTitle(m_Window, title);
    }
}