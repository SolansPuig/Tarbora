#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wextra"
#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    class GraphicsEngine;

    class Gui
    {
    public:
        Gui(GraphicsEngine *graphicsEngine);
        ~Gui();

        void BeforeDraw();
        void AfterDraw();

    private:
        GraphicsEngine *m_GraphicsEngine;
    };
}
