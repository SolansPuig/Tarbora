#pragma once
#include "Layer.hpp"

namespace Tarbora {
    class DemoWindow : public Layer
    {
    public:
        DemoWindow(GraphicView *app, bool start_active) : Layer(app, start_active)
        {}

        void GetInput() override
        {
            if (app->Input()->GetKeyDown(KEY_F5)) {
                m_active = !m_active;
            }
        }

        void Draw() override
        {
            ImGui::ShowDemoWindow(&m_active);
        }
    };
}
