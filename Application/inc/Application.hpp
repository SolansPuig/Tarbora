#pragma once
#include "Game_View.hpp"
#include "Settings.hpp"

namespace Tarbora {
    class Application
    {
    public:
        Application(const char *settings_path);
        ~Application();

        void Run();
        void Update();
        void Draw();

        void Close() { m_run = false; }

        void AddView(GameViewPtr view);

        float GetElapsedTime() { return m_elapsed_time; }
    private:
        bool m_run;
        float m_time;
        float m_elapsed_time;
        GameViewList m_Game_Views;
    };
}
