#include "../inc/Application.hpp"

namespace Tarbora {
    Application::Application(std::string logFile) : m_run(true), m_time(0.0f)
    {
        if (logFile != "" || !Logger::Init(logFile))
        {
            Logger::Init(stdout);
        }
        LOG_LEVEL(Tarbora::Logger::LogLevel::INFO);
        LOG_INFO("Application: Creating application...");

        ResourceManager::Init("../Resources/");

        GraphicsEngine::Init();
        PhysicsEngine::Init();
        Input::Init();
        m_Actors.Init(1000);

        LOG_INFO("Application: Successfully created application");
    }

    Application::~Application()
    {
        LOG_INFO("Application: Destroying application...");

        m_Actors.Close();

        PhysicsEngine::Close();
        GraphicsEngine::Close();

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
        float deltaTime = m_time > 0.0 ? (current_time - m_time) : (1.0f/60.0f);
        m_time = current_time;
        PhysicsEngine::Update(deltaTime);
        m_Actors.Update(deltaTime);
        for (auto &itr : m_Game_Views)
        {
            itr->Update(deltaTime);
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
