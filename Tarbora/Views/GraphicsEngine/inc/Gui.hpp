#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wextra"
#include "../imgui/imgui.h"
#include "../imgui/misc/cpp/imgui_stdlib.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    class GraphicView;

    class Gui
    {
    public:
        Gui(GraphicView *app);
        ~Gui();

        void BeforeDraw();
        void AfterDraw();

    private:
        GraphicView *app;
    };
}
