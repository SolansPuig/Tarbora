#pragma once
#include "GameView.hpp"
#include "Settings.hpp"
#include "ResourceManager.hpp"
#include "Actors.hpp"

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

        Actors m_Actors;
    };
}
