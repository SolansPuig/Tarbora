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
    }
}
