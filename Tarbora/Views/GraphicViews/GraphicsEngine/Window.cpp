#include "GraphicsEngine.hpp"
#include "../../../Framework/Module/Module.hpp"
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../Framework/External/stb_image_write.h"
#include "../../../Framework/External/tracy/TracyOpenGL.hpp"

namespace Tarbora {
    Window::Window(const std::string &title, int width, int height, GraphicsEngine *graphics_engine) :
        props_(WindowProps(title, width, height, graphics_engine))
    {
        LOG_DEBUG("Creating window with title: %s, width: %d and height: %d", props_.title.c_str(), props_.width, props_.height);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(props_.width, props_.height, props_.title.c_str(), NULL, NULL);
        if (window_ == NULL) {
            LOG_ERR("Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window_);

        glfwSetWindowUserPointer(window_, &props_);

        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
		{
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
            data.ratio = (float)width / (float)height;

            glViewport(0, 0, width, height);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
		{
            WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
            data.graphics_engine->getModule()->close();
        });

        stbi_flip_vertically_on_write(1);
    }

    Window::~Window()
    {
        if (window_)
            glfwDestroyWindow(window_);
    }

    void Window::close()
    {
        if (window_)
            glfwDestroyWindow(window_);
        else
            LOG_ERR("Trying to close an unexisting GLFW window");
    }

    void Window::update()
    {
        glfwPollEvents();
        glfwSwapBuffers(window_);
    }

    void Window::captureMouse(bool capture)
    {
        glfwSetInputMode(window_, GLFW_CURSOR, (capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));
    }

    void Window::setTitle(const std::string &title)
    {
        glfwSetWindowTitle(window_, title.c_str());
    }

    int Window::takeScreenshot(const std::string &filename)
    {
        // Get the window width and height and reserve memory
        int width = props_.width;
        int height = props_.height;
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
