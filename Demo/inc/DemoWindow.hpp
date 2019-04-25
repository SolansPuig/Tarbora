#pragma once
#include "../../Tarbora/Tarbora.hpp"

namespace Tb = Tarbora;

class DemoWindow : public Tb::Layer
{
    float m_dt;
public:
    DemoWindow(bool start_active) : Tb::Layer(start_active)
    {}

    void Draw() override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
    }
};
