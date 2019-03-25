#pragma once
#include "../../Tarbora.hpp"

namespace Tb = Tarbora;

class TestGui : public Tb::Layer
{
    float m_dt;
public:
    ImTextureID m_Id;
    ImVec2 m_Size;
    TestGui(bool start_active) : Tb::Layer(start_active)
    {
        // std::shared_ptr<Tb::Texture> tex = GET_RESOURCE(Tb::Texture, "textures/sky.png");
        // m_Id = (ImTextureID)tex->GetId();
        // m_Size = ImVec2(tex->GetWidth()/4.0, tex->GetHeight()/4.0);
    }

    void Draw() override
    {
        static bool open = true;
        static float slider = 0.0f;
        static char text[32] = "Placeholder";
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f));

        if (open)
        {
            ImGui::Begin("Test Gui", &open);
            ImGui::Text("Testing Gui");
            ImGui::Separator();

            if (ImGui::Button("Log!"))
            {
                LOG_INFO("Test Gui: Log!");
            }

            ImGui::SliderFloat("Slider!", &slider, 0.0f, 10.0, "%.3f", 1.0f);
            if (ImGui::Button("Log Slider!"))
            {
                LOG_INFO("Test Gui: Slider: %.3f!", slider);
            }

            ImGui::InputText("Text!", text, 32);
            if (ImGui::Button("Log Text!"))
            {
                LOG_INFO("Test Gui: Text: %s!", text);
            }

            // ImGui::Image(m_Id, m_Size);

            ImGui::End();
        }

    }
};
