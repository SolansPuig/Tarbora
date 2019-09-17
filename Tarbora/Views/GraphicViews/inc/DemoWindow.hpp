#pragma once
#include "Layer.hpp"

namespace Tarbora {
    class DemoWindow : public Layer
    {
    public:
        DemoWindow(GraphicView *view, bool start_active) : Layer(view, start_active)
        {}

        void GetInput() override
        {
            if (m_View->Input()->GetKeyDown(KEY_F5)) {
                m_active = !m_active;
            }
        }

        void Draw() override
        {
            ImGui::ShowDemoWindow(&m_active);
        }
    };
}
