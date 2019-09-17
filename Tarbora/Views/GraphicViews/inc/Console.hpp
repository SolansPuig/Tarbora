#pragma once
#include "Layer.hpp"
#include <ctime>

namespace Tarbora {
    class ConsoleImpl : public Layer
    {
        typedef std::function<void(std::string&)> InputCallback;

    public:
        ConsoleImpl(GraphicView *view, bool start_active, bool fullWindow, InputCallback callback)
            : Layer(view, start_active), m_fullWindow(fullWindow), m_callback(callback)
        {
        }

        void GetInput() override
        {
            if (m_View->Input()->GetKeyDown(KEY_F4) && !m_fullWindow) {
                m_active = !m_active;
            }
        }

        void Log(std::string text)
        {
            time_t now = time(0);
            tm *ltm = localtime(&now);

            char hour[12];
            sprintf(hour, "[%02d:%02d:%02d] ", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

            m_log.push_back(hour + text);
        }

        void Draw() override
        {
            static std::string input("");

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowViewport(viewport->ID);

            if (m_fullWindow) {
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
            } else {
                ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f));
            }

            if (m_active)
            {
                ImGui::Begin("ConsoleImpl", &m_active, m_fullWindow ? ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_None);

                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

                for (auto &message : m_log)
                {
                    ImGui::TextUnformatted(message.c_str());
                }

                ImGui::EndChild();
                ImGui::Separator();

                if (ImGui::InputText("Input", &input, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    m_callback(input);
                    input.clear();
                    ImGui::SetKeyboardFocusHere(-1);
                }

                ImGui::SetItemDefaultFocus();

                ImGui::End();
            }

        }

    private:
        std::vector<std::string> m_log;
        std::vector<std::string> m_history;
        bool m_fullWindow;
        InputCallback m_callback;
    };
}
