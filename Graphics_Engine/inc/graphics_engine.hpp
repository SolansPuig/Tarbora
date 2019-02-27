#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>

#include "window.hpp"
#include "shader.hpp"

#include "node.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Graphics_Engine {
public:
    Graphics_Engine();
    ~Graphics_Engine();

    void Initialize();
    
    void BeforeDraw();
    void Draw(Node* node);
    void AfterDraw();

    Window * GetWindow();

    Node * UseNode();
    void ReleaseNode(Node * node);

    bool ShouldClose();
    void SetWindowTitle(const char* title);

    Node * LoadModel(const char* filePath, int tex_size);
    unsigned int LoadTexture(const char* filePath, int * tex_size);

    void DeleteModel(Node * node);
private:
    float clearR=0.14, clearG=0.16, clearB = 0.19; // The color that the Main Window will clear to
    Window Main_Window;
    Shader Main_Shader;

    NodePool Node_Pool;

    Node * LoadNode(auto itr, std::string * name, float pyxel_density, int tex_size);
};

#endif
