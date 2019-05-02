#pragma once
#include "../../Framework/Framework.hpp"
#include "../../GameView/GameView.hpp"
#include "../../GameLogic/GameLogic.hpp"

namespace Tarbora {
    class Application
    {
    public:
        Application(std::string logFile="");
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
