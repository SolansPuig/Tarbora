#pragma once
#include "Window.hpp"
#include "Layer.hpp"

namespace Tarbora {
    namespace Graphics_Engine {
        void Init();
        void Close();

        void BeforeDraw();
        void AfterDraw();

        WindowPtr GetWindow();
    }
}
