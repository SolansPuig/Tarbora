#include "HumanView.hpp"
#include "Events.hpp"
#include "GraphicsEngine.hpp"

namespace Tarbora {
    HumanView::HumanView(ActorId id)
    {
        EventFn onEvent = [this](Event* e)
        {
            for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
            {
                if ((*itr)->OnEvent(e))
                    break;
            }
        };
        EvtKeyPressId = EventManager::Subscribe(EventType::KeyPress, onEvent);
        EvtKeyReleaseId = EventManager::Subscribe(EventType::KeyRelease, onEvent);
        EvtButtonPressId = EventManager::Subscribe(EventType::MouseButtonPress, onEvent);
        EvtButtonReleaseId = EventManager::Subscribe(EventType::MouseButtonRelease, onEvent);
        EvtMouseMoveId = EventManager::Subscribe(EventType::MouseMove, onEvent);
        EvtMouseScrollId = EventManager::Subscribe(EventType::MouseScroll, onEvent);

        m_GameLayer.reset(new GameLayer(true));
        PushLayer(m_GameLayer);
        m_GameLayer->SetTargetId(id);
    }

    HumanView::~HumanView()
    {
        EventManager::Unsubscribe(EventType::KeyPress, EvtKeyPressId);
        EventManager::Unsubscribe(EventType::KeyRelease, EvtKeyReleaseId);
        EventManager::Unsubscribe(EventType::MouseButtonPress, EvtButtonPressId);
        EventManager::Unsubscribe(EventType::MouseButtonRelease, EvtButtonReleaseId);
        EventManager::Unsubscribe(EventType::MouseMove, EvtMouseMoveId);
        EventManager::Unsubscribe(EventType::MouseScroll, EvtMouseScrollId);
    }

    void HumanView::Update(float elapsed_time)
    {
        for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
        {
            if ((*itr)->IsActive())
                (*itr)->Update(elapsed_time);
        }
    }

    void HumanView::Draw()
    {
        GraphicsEngine::BeforeDraw();

        for (auto &itr : m_Layers)
        {
            if (itr->IsActive())
                itr->Draw();
        }

        GraphicsEngine::AfterDraw();
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
