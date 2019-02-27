#include "graphics_engine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Graphics_Engine::Graphics_Engine(){}
Graphics_Engine::~Graphics_Engine() {
    glfwTerminate();
}

void Graphics_Engine::Initialize() {
    // Initialize GLFW
    glfwInit();

    // Open the Main Window
    Main_Window.Open("Main Window", 800, 600);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "GRAPHICS_ENGINE::ERROR::Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Compile the shader programs
    Main_Shader.LoadFromFile("../resources/shaders/mainShader.vs", "../resources/shaders/mainShader.fs");

    // Configure the main shader
    Main_Shader.Use();
    Main_Shader.SetFloatVec3("objectColor", 1.0f, 0.5f, 0.31f);
}

void Graphics_Engine::BeforeDraw() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)Main_Window.GetWidth() / (float)Main_Window.GetHeight(), 0.1f, 100.0f);

    // Select the shader that we will use
    Main_Shader.Use();

    Main_Shader.SetMat4("view", view);
    Main_Shader.SetMat4("projection", projection);

    // Clear the Window to a solid color in order to draw again
    Main_Window.Clear(clearR, clearG, clearB);
}

void Graphics_Engine::Draw(Node* node) {
    node->Draw(&Main_Shader);
}

void Graphics_Engine::AfterDraw() {
    // Apply changes
    Main_Window.Update();
}

Window * Graphics_Engine::GetWindow() {
    return &Main_Window;
}

bool Graphics_Engine::ShouldClose() {
    return Main_Window.ShouldClose();
}

void Graphics_Engine::SetWindowTitle(const char* title) {
    Main_Window.SetTitle(title);
}

// TODO: Aquesta funció ha d'anar a un gestor de recursos, no al motor gràfic
// Note: This function is from learnopengl.com, with some modifications
unsigned int Graphics_Engine::LoadTexture(char const * path, int * tex_size) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int nrComponents, height;
    unsigned char *data = stbi_load(path, tex_size, &height, &nrComponents, 0);
    if (*tex_size == height && data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, *tex_size, *tex_size, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Node* Graphics_Engine::LoadModel(const char* filePath, int tex_size) {
    // Parse the JSON file
    std::ifstream ifs(filePath);
    json j = json::parse(ifs);

    // Config the model
    float pyxel_density = j["pyxel_density"];

    // Read the root node
    std::string name;
    return LoadNode(&j["root"], &name, pyxel_density, tex_size);
}

Node* Graphics_Engine::LoadNode(auto n, std::string * name, float pyxel_density, int tex_size) {
    // Read all the configuration from the JSON
    *name = n->at("name");
    std::string shape = n->at("shape");
    bool mirror = n->at("mirror");
    json origin = n->at("origin");
    json pos = n->at("position");
    json rotation = n->at("rotation");
    json size = n->at("size");
    json uv = n->at("uv");

    Node* node = Node_Pool.Use();

    // Define the shape and the UV map to the new node
    node->SetShape((mirror ? "M_" : "") + shape);
    if (shape == "PLANE") { // Planes need to substract their "z" size from the UV for code versality
        float z = size[2];
        node->SetUV(
            (float)size[0] / tex_size,
            (float)size[1] / tex_size,
            (float)size[2] / tex_size,
            ((float)uv[0] - z) / tex_size,
            ((float)uv[1] - z) / tex_size);
    } else {
        node->SetUV(
            ((float)size[0]) / tex_size,
            ((float)size[1]) / tex_size,
            ((float)size[2]) / tex_size,
            (float)uv[0] / tex_size,
            (float)uv[1] / tex_size);
    }
    // Set the coordinates
    node->SetOrigin(origin[0], origin[1], origin[2]);
    node->Translate((float)pos[0] / pyxel_density, (float)pos[1] / pyxel_density, (float)pos[2] / pyxel_density);
    node->Scale((float)size[0] / pyxel_density, (float)size[1] / pyxel_density, (float)size[2] / pyxel_density);
    node->Rotate(rotation[0], rotation[1], rotation[2], rotation[3]);

    // Create all its child nodes and add them as children to this
    for (auto itr = n->at("nodes").begin(); itr != n->at("nodes").end(); itr++) {
        std::string new_name;
        Node* new_node = LoadNode(itr, &new_name, pyxel_density, tex_size);
        node->AddChild(new_node, new_name.c_str());
    }

    return node;
}

void Graphics_Engine::DeleteModel(Node * node) {
    Node_Pool.Release(node);
}
