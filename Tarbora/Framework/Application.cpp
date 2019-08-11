#include "Application.hpp"
#include "Global.hpp"

namespace Tarbora {
    Application::Application()
    {
        m_using_thread = false;
    }

    Application::~Application()
    {
        if (m_using_thread) m_thread.join();
    }

    void Application::RunThread()
    {
        m_using_thread = true;
        m_thread = std::thread(&Application::Run, this);
    }

    void Application::Close()
    {
        Application::m_run = false;
    }

    bool Application::m_run = true;
}
