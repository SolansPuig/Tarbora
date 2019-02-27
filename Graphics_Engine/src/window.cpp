#include <iostream>
#include "window.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    (void)(window);
    glViewport(0, 0, width, height);
}

Window::Window() {}
Window::~Window() {
    if (m_Window)
        glfwDestroyWindow(m_Window);
}

void Window::Open(const char* title, int width, int height) {
    m_title = title;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, m_title, NULL, NULL);
    if (m_Window == NULL) {
        std::cerr << "GRAPHICS_ENGINE::WINDOW::ERROR::Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_Window);

    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
}

void Window::Close() {
    if (m_Window)
        glfwDestroyWindow(m_Window);
    else
        std::cerr << "GRAPHICS_ENGINE::WINDOW::ERROR::The window is already closed" << std::endl;
}

void Window::Update() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

int Window::GetWidth() {
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);
    return width;
}

int Window::GetHeight() {
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);
    return height;
}

void Window::Clear(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_Window);
}

void Window::SetTitle(const char* title) {
    glfwSetWindowTitle(m_Window, title);
}

bool Window::IsPressed(int key) {
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Window::IsReleased(int key) {
    return glfwGetKey(m_Window, key) == GLFW_RELEASE;
}

// void Window::SetFullscreen(bool fullScreen) {
//
// }
