#pragma once
#include "Window.hpp"
#include "Layer.hpp"
// #include "shader.hpp"

namespace Tarbora {
    namespace Graphics_Engine {
        void Init();
        void Close();

        void BeforeDraw();
        void AfterDraw();

        WindowPtr GetWindow();
        // Shader *GetShader();

        unsigned int CompileShader(std::string type, const char *code);
        unsigned int LinkProgram(unsigned int *ids);
        void DeleteProgram(unsigned int id);
    }
}
