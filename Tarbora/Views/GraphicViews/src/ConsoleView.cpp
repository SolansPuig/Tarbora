#include "../inc/ConsoleView.hpp"
#include "../inc/GameLayer.hpp"
#include "../inc/DemoWindow.hpp"
#include "../inc/MetricsGui.hpp"
#include "../inc/Console.hpp"

namespace Tarbora {
    ConsoleView::ConsoleView(std::string server_address)
        : GraphicView(21, server_address, "ConsoleSettings.json")
    {
        LOG_DEBUG("Creating console view...");

        m_Console = std::make_shared<ConsoleImpl>(this, true, true, [this](std::string &input)
        {
            Console()->Log(input);
        });

        m_All = MessageManager()->Subscribe("all", [this](std::string subject, std::string body)
        {
            m_Console->Log(subject);
        });
    }

    ConsoleView::~ConsoleView()
    {
        LOG_DEBUG("Destroying console game view...");

        MessageManager()->Desubscribe("all", m_All);
    }

    void ConsoleView::GetInput()
    {
        if (Input()->GetKeyDown(KEY_ESCAPE)) {
            Close();
        }
    }

    void ConsoleView::Update(float elapsed_time)
    {
        m_Console->Update(elapsed_time);
    }

    void ConsoleView::Draw()
    {

        GraphicsEngine()->BeforeDraw();

        m_Console->Draw();

        GraphicsEngine()->AfterDraw();
    }
}
