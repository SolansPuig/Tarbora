#include "Application.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Events.hpp"
#include "KeyCodes.hpp"
#include "Input.hpp"
#include "HumanView.hpp"
#include "Settings.hpp"

namespace Tarbora {
    Application::Application(const char *settings_path) : m_run(true), m_time(0.0f)
    {
        Logger::Init(stdout);
        LOG_LEVEL(Tarbora::Logger::LogLevel::DEBUG);
        LOG_INFO("Application: Creating application...");

        ResourceManager::Init(1024, "../resources/");
        Settings::Load(settings_path);
        Graphics_Engine::Init();
        Input::Init();

        LOG_INFO("Application: Successfully created application");
    }

    Application::~Application()
    {
        LOG_INFO("Application: Destroying application...");

        Graphics_Engine::Close();

        LOG_INFO("Application: Successfully destroyed application");
        Logger::Close();
    }

    void Application::Run()
    {
        while(m_run)
        {
            Update();
            Draw();
        }
    }

    void Application::Update()
    {
        float current_time = (float)glfwGetTime();
        float m_elapsed_time = m_time > 0.0 ? (current_time - m_time) : (1.0f/60.0f);
        m_time = current_time;
        // m_Metrics.SetTime(elapsed_time);
        for (auto &itr : m_Game_Views)
        {
            itr->Update(m_elapsed_time);
        }
    }

    void Application::Draw()
    {
        for (auto &itr : m_Game_Views)
        {
            itr->Draw();
        }
    }

    void Application::AddView(GameViewPtr view)
    {
        m_Game_Views.push_back(view);
    }
}
