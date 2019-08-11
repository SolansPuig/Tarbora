#pragma once
#include <thread>

namespace Tarbora {
    class Application
    {
    public:
        Application();
        ~Application();

        void RunThread();
        virtual void Run() = 0;

        void Close();

    protected:
        static bool m_run;

    private:
        std::thread m_thread;
        bool m_using_thread;
    };
}
