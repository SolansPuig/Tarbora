#pragma once
#include "Layer.hpp"

namespace Tarbora {
    class MetricsGui : public Layer
    {
    public:
        MetricsGui(GraphicView *view, bool start_active)
            : Layer(view, start_active), delta_time_(0.0f)
        {}

        void getInput() override
        {
            if (getInputManager()->getKeyDown(KEY_F3)) {
                active_ = !active_;
            }
        }

        void update(float delta_time) override
        {
            delta_time_ = delta_time;
        }

        void draw() override
        {
            const float DISTANCE = 10.f;

            ImVec2 window_pos = ImVec2(ImGui::GetWindowPos().x + DISTANCE, ImGui::GetWindowPos().y + DISTANCE);
            ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowBgAlpha(0.3f);
            ImGui::Begin("Metrics", &active_, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
            ImGui::Text("FPS: %.0f", 1/delta_time_);
            ImGui::End();
        }

        void SetTime(float delta_time)
        {
            delta_time_ = delta_time * 1000;
        }

    private:
        float delta_time_;
    };
}
