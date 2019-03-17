#pragma once
#include "Game_View.hpp"

namespace Tarbora {
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void Update();
        void Draw();
    private:
        bool m_run;
        float m_time;
        GameViewList m_Game_Views;
    };
}
