#pragma once
#include "../../Tarbora.hpp"

namespace Tb = Tarbora;

class MetricsGui : public Tb::Layer
{
    float m_elapsed_time;
public:
    MetricsGui(bool start_active) : Tb::Layer(start_active), m_elapsed_time(0.0f)
    {}

    void Update(float elapsed_time) override
    {
        m_elapsed_time = elapsed_time * 1000;
    }

    void Draw() override
    {
        const float DISTANCE = 10.0f;
        bool open = true;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 window_pos = ImVec2(viewport->Pos.x + DISTANCE, viewport->Pos.y + DISTANCE);
        ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("Metrics", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
        ImGui::Text("FPS: %.0f", 1000/m_elapsed_time);
        ImGui::End();
    }

    void SetTime(float elapsed_time)
    {
        m_elapsed_time = elapsed_time * 1000;
    }
};
