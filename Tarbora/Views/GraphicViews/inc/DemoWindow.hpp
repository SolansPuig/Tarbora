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
            if (static_cast<GraphicView*>(m_Module)->Input()->GetKeyDown(KEY_F5)) {
                m_active = !m_active;
            }
        }

        void Draw() override
        {
            ImGui::ShowDemoWindow(&m_active);
        }
    };
}
