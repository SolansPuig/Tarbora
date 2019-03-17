#pragma once
#include "Metrics_Gui.hpp"

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
        Metrics_Gui m_Metrics;
    };
}
