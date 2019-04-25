#pragma once
#include "../../Global.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include "../glfw/include/GLFW/glfw3.h"
#define GLM_FORCE_CXX98

namespace Tarbora {
    struct WindowProps {
        const char* m_title;
        int m_width;
        int m_height;
        float m_ratio;

        WindowProps(const char *title, int width, int height) :
            m_title(title), m_width(width), m_height(height), m_ratio((float)width/(float)height) {};
    };

    class Window
    {
    public:
        Window(const char* title, int width, int height);
        ~Window();

        void SetTitle(const char* title);

        void Close();
        void Update();
        void Clear();

        int GetWidth() { return m_Props.m_width; }
        int GetHeight() { return m_Props.m_height; }
        float GetRatio() { return m_Props.m_ratio; }

        GLFWwindow* GetWindow() { return m_Window; }
    private:
        GLFWwindow* m_Window;
        WindowProps m_Props;
    };

    typedef std::shared_ptr<Window> WindowPtr;
}
