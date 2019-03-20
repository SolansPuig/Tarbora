#pragma once
#include "../../Tarbora.hpp"

namespace Tb = Tarbora;

class DemoWindow : public Tb::Layer
{
    float m_dt;
public:
    DemoWindow() : Tb::Layer()
    {}

    void Draw() override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
    }
};
