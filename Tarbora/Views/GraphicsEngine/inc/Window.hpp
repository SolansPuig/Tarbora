#pragma once
#include "../../../Framework/Global.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include "../glfw/include/GLFW/glfw3.h"
#define GLM_FORCE_CXX98

namespace Tarbora {
    class GraphicView;

    struct WindowProps {
        const char* title;
        int width;
        int height;
        float ratio;
        GraphicView *view;

        WindowProps(const char *title, int width, int height, GraphicView *view) :
            title(title), width(width), height(height), ratio((float)width/(float)height), view(view) {};
    };

    class Window
    {
    public:
        Window(const char* title, int width, int height, GraphicView *view);
        ~Window();

        void SetTitle(const char* title);

        void Close();
        void Update();

        void CaptureMouse(bool capture);

        int GetWidth() { return m_Props.width; }
        int GetHeight() { return m_Props.height; }
        float GetRatio() { return m_Props.ratio; }

        GLFWwindow* GetRawWindow() { return m_Window; }
    private:
        GLFWwindow* m_Window;
        WindowProps m_Props;
    };

    typedef std::shared_ptr<Window> WindowPtr;
}
