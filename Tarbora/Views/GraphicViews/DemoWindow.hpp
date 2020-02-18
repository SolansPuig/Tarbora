#pragma once
#include "Layer.hpp"

namespace Tarbora {
    class DemoWindow : public Layer
    {
    public:
        DemoWindow(GraphicView *view, bool start_active) : Layer(view, start_active)
        {}

        void getInput() override
        {
            if (getInputManager()->getKeyDown(KEY_F6)) {
                active_ = !active_;
            }
        }

        void draw() override
        {
            ImGui::ShowDemoWindow(&active_);
        }
    };
}
