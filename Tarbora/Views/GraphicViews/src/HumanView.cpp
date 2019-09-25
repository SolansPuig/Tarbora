#include "../inc/HumanView.hpp"
#include "../inc/DemoWindow.hpp"
#include "../inc/MetricsGui.hpp"

namespace Tarbora {
    HumanView::HumanView(std::string server_address) :
        GraphicView(20, server_address, "Settings.json")
    {
        LOG_DEBUG("Creating human game view...");

        m_MaxFrameRate = 60;

        m_GameLayer = std::make_shared<GameLayerImpl>(this, true);
        m_GameLayer->SetTargetId(5);
        PushLayer(m_GameLayer);

        std::shared_ptr<MetricsGui> metrics(new MetricsGui(this, false));
        PushLayer(metrics);

        m_Console = std::make_shared<ConsoleImpl>(this, false, false, [this](std::string &input)
        {
            Console()->Log(input);
        });

        PushLayer(m_Console);

        GraphicsEngine()->GetWindow()->CaptureMouse(true);

        LOG_DEBUG("Created");
        // std::shared_ptr<DemoWindow> demo_gui(new DemoWindow(this, false));
        // PushLayer(demo_gui);
    }

    HumanView::~HumanView()
    {
        LOG_DEBUG("Destroying human game view...");
    }

    void HumanView::GetInput()
    {
        ZoneScoped;

        if (Input()->GetKeyDown(KEY_ESCAPE)) {
            static bool capture = true;
            capture = !capture;
            GraphicsEngine()->GetWindow()->CaptureMouse(capture);
        }

        if (Input()->GetKeyDown(KEY_F2)) {
            GraphicsEngine()->TakeScreenshot("/home/roger/Imatges/test");
        }

        if (Input()->GetKeyDown(KEY_F5)) {
            ResourceManager::Flush();
        }

        for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
        {
            (*itr)->GetInput();
        }
    }

    void HumanView::Update(float elapsed_time)
    {
        ZoneScoped;

        for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
        {
            if ((*itr)->IsActive())
                (*itr)->Update(elapsed_time);
        }
    }

    void HumanView::Draw()
    {
        ZoneScoped;

        GraphicsEngine()->BeforeDraw();

        for (auto &itr : m_Layers)
        {
            if (itr->IsActive())
                itr->Draw();
        }

        GraphicsEngine()->AfterDraw();
    }

    void HumanView::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.push_back(layer);
    }

    void HumanView::RemoveLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.remove(layer);
    }
}
