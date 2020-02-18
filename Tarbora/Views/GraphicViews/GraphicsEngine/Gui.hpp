#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wextra"
#include "External/imgui/imgui.h"
#include "External/imgui/misc/cpp/imgui_stdlib.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    class GraphicsEngine;

    class Gui
    {
    public:
        Gui(GraphicsEngine *graphicsEngine);
        ~Gui();

        void beforeDraw();
        void afterDraw();

    private:
        GraphicsEngine *graphics_engine_;
    };
}
