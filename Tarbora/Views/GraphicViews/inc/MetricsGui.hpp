#pragma once
#include "Layer.hpp"

namespace Tarbora {
    class MetricsGui : public Layer
    {
    public:
        MetricsGui(GraphicView *view, bool start_active)
            : Layer(view, start_active), m_elapsed_time(0.0f)
        {}

        void GetInput() override
        {
            if (m_View->Input()->GetKeyDown(KEY_F3)) {
                m_active = !m_active;
            }
        }

        void Update(float elapsed_time) override
        {
            m_elapsed_time = elapsed_time;
        }

        void Draw() override
        {
            const float DISTANCE = 10.0f;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 window_pos = ImVec2(viewport->Pos.x + DISTANCE, viewport->Pos.y + DISTANCE);
            ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::SetNextWindowBgAlpha(0.3f);
            ImGui::Begin("Metrics", &m_active, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
            ImGui::Text("FPS: %.0f", 1/m_elapsed_time);
            ImGui::End();
        }

        void SetTime(float elapsed_time)
        {
            m_elapsed_time = elapsed_time * 1000;
        }

    private:
        float m_elapsed_time;
    };
}
