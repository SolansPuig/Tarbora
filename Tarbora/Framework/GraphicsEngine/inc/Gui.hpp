#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wextra"
#include "../imgui/imgui.h"
#pragma GCC diagnostic pop

namespace Tarbora {
    class Gui
    {
    public:
        Gui();
        ~Gui();

        void BeforeDraw();
        void AfterDraw();
    };
}
