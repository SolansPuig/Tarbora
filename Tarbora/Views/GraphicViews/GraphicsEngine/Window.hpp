#pragma once
#include "../../../Framework/Global.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
// #include <GL/glext.h>
#include "glfw/include/GLFW/glfw3.h"
#define GLM_FORCE_CXX98

namespace Tarbora {
    class GraphicsEngine;

    struct WindowProps {
        const char* title;
        int width;
        int height;
        float ratio;
        GraphicsEngine *graphicsEngine;

        WindowProps(const char *title, int width, int height, GraphicsEngine *graphicsEngine) :
            title(title), width(width), height(height), ratio((float)width/(float)height), graphicsEngine(graphicsEngine) {};
    };

    class Window
    {
    public:
        Window(const char* title, int width, int height, GraphicsEngine *graphicsEngine);
        ~Window();

        void SetTitle(const char* title);

        void Close();
        void Update();

        void CaptureMouse(bool capture);

        int GetWidth() { return m_Props.width; }
        int GetHeight() { return m_Props.height; }
        float GetRatio() { return m_Props.ratio; }

        GLFWwindow* GetRawWindow() { return m_Window; }

        int TakeScreenshot(const std::string &filename);
    private:
        GLFWwindow* m_Window;
        WindowProps m_Props;
    };
}
