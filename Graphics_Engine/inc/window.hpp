#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

    void Open(const char* title, int width, int height);
    void Close();
    void Update();
    void Clear(float r, float g, float b);

    int GetWidth();
    int GetHeight();

    bool ShouldClose();

    void SetTitle(const char* title);
    // void SetFullscreen(bool fullScreen);

    bool IsPressed(int key);
    bool IsReleased(int key);

private:
    GLFWwindow* m_Window;
    const char* m_title;
};

#endif
