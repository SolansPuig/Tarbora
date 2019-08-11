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
        GraphicView *app;

        WindowProps(const char *title, int width, int height, GraphicView *app) :
            title(title), width(width), height(height), ratio((float)width/(float)height), app(app) {};
    };

    class Window
    {
    public:
        Window(const char* title, int width, int height, GraphicView *app);
        ~Window();

        void SetTitle(const char* title);

        void Close();
        void Update();
        void Clear();

        int GetWidth() { return m_Props.width; }
        int GetHeight() { return m_Props.height; }
        float GetRatio() { return m_Props.ratio; }

        GLFWwindow* GetWindow() { return m_Window; }
    private:
        GLFWwindow* m_Window;
        WindowProps m_Props;
    };

    typedef std::shared_ptr<Window> WindowPtr;
}
