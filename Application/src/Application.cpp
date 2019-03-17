#include "Application.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Events.hpp"
#include "KeyCodes.hpp"
#include "Input.hpp"
#include "Demo_Window.hpp"
#include "Test_Gui.hpp"

namespace Tarbora {
    Application::Application() : m_run(true), m_time(0.0f)
    {
        Logger::Init(stdout);
        LOG_LEVEL(Tarbora::Logger::LogLevel::DEBUG);
        LOG_INFO("Application: Creating application...");

        Graphics_Engine::Init();
        Input::Init();

        Graphics_Engine::AddGuiLayer(m_Metrics);
        m_Metrics.SetActive(false);
        Test_Gui* test = new Test_Gui();
        Graphics_Engine::AddGuiLayer(*test);
        // Demo_Window* demo = new Demo_Window();
        // Graphics_Engine::AddGuiLayer(*demo);

        Event::WindowClose.Subscribe([this](WindowCloseEvent& e) {
            (void)(e);
            m_run = false;
        });

        Event::KeyRelease.Subscribe([this](KeyReleaseEvent& e)
        {
            if (e.key == KEY_F3) m_Metrics.SetActive(!m_Metrics.IsActive());
            else if (e.key == KEY_ESCAPE) m_run = false;
        });

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
        float dt = m_time > 0.0 ? (current_time - m_time) : (1.0f/60.0f);
        m_time = current_time;
        m_Metrics.SetTime(dt);
    }

    void Application::Draw()
    {
        Graphics_Engine::BeforeDraw();
        Graphics_Engine::Draw();
        Graphics_Engine::AfterDraw();
    }
}
