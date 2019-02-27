#include "Graphics_Engine/inc/graphics_engine.hpp"
#include <iostream>
#include <ctime>
#include <cstdio>

static int rotation = 0;

void processInput(Window* window) {
    if (window->IsPressed(GLFW_KEY_E)) rotation = -1;
    else if (window->IsPressed(GLFW_KEY_T)) rotation = 1;
    else rotation = 0;
}

int main() {
    Graphics_Engine Graphics;
    Graphics.Initialize();
    double fps;
    double elapsed_time;
    char title[50];

    int tex_size;
    unsigned int tex = Graphics.LoadTexture("../resources/textures/male.png", &tex_size);
    glBindTexture(GL_TEXTURE_2D, tex);
    Node* root = Graphics.LoadModel("../resources/models/human.json", tex_size);

    while (!Graphics.ShouldClose()) {
        double begin = glfwGetTime();
        processInput(Graphics.GetWindow());
        Graphics.BeforeDraw();

        Graphics.Draw(root);
        root->Rotate(rotation, 0, 1, 0);

        Graphics.AfterDraw();
        double end = glfwGetTime();
        elapsed_time = end - begin;
        fps = 1 / elapsed_time;
        sprintf(title, "FPS: %lf, ELAPSED TIME: %lf", fps, elapsed_time);
        Graphics.SetWindowTitle(title);
    }

    std::cout << "Bye!" << std::endl;

    return 0;
}
