#include "HumanView.hpp"
#include "Events.hpp"
#include "Graphics_Engine.hpp"

namespace Tarbora {
    HumanView::HumanView()
    {
    }

    HumanView::~HumanView()
    {
    }

    void HumanView::OnCreate(GameViewId id)
    {
        m_Id = id;

        EventFn onEvent = [this](Event* e)
        {
            for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
            {
                if ((*itr)->OnEvent(e))
                    break;
            }
        };

        EvtKeyPressId = Event_Manager::Subscribe(EventType::KeyPress, onEvent);
        EvtKeyReleaseId = Event_Manager::Subscribe(EventType::KeyRelease, onEvent);
        EvtButtonPressId = Event_Manager::Subscribe(EventType::MouseButtonPress, onEvent);
        EvtButtonReleaseId = Event_Manager::Subscribe(EventType::MouseButtonRelease, onEvent);
        EvtMouseMoveId = Event_Manager::Subscribe(EventType::MouseMove, onEvent);
        EvtMouseScrollId = Event_Manager::Subscribe(EventType::MouseScroll, onEvent);
    }

    void HumanView::OnDestroy()
    {
        Event_Manager::Unsubscribe(EventType::KeyPress, EvtKeyPressId);
        Event_Manager::Unsubscribe(EventType::KeyRelease, EvtKeyReleaseId);
        Event_Manager::Unsubscribe(EventType::MouseButtonPress, EvtButtonPressId);
        Event_Manager::Unsubscribe(EventType::MouseButtonRelease, EvtButtonReleaseId);
        Event_Manager::Unsubscribe(EventType::MouseMove, EvtMouseMoveId);
        Event_Manager::Unsubscribe(EventType::MouseScroll, EvtMouseScrollId);
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
        Graphics_Engine::BeforeDraw();

        for (auto &itr : m_Layers)
        {
            if (itr->IsActive())
                itr->Draw();
        }

        Graphics_Engine::AfterDraw();
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
