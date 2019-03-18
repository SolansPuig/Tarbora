#pragma once
#include "../../Tarbora.hpp"

namespace Tb = Tarbora;

class Demo_Window : public Tb::Layer
{
    float m_dt;
public:
    Demo_Window() : Tb::Layer()
    {}

    void Draw() override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
    }
};
