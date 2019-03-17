#pragma once
#include "Layer.hpp"
#include "imgui.h"

namespace Tarbora {
    class Demo_Window : public Layer
    {
        float m_dt;
    public:
        Demo_Window() : Layer()
        {}

        void Draw() override
        {
            bool open = true;
            ImGui::ShowDemoWindow(&open);
        }
    };
}
