#include "Human_View.hpp"
#include "Events.hpp"
#include "Graphics_Engine.hpp"

namespace Tarbora {
    Human_View::Human_View(float r, float g, float b)
    {
        Graphics_Engine::GetWindow()->SetClearColor(r, g, b);
    }

    Human_View::~Human_View()
    {
    }

    void Human_View::OnCreate(GameViewId id)
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

        EvtKeyPressId = EventManager::Subscribe(EventType::KeyPress, onEvent);
        EvtKeyReleaseId = EventManager::Subscribe(EventType::KeyRelease, onEvent);
        EvtButtonPressId = EventManager::Subscribe(EventType::MouseButtonPress, onEvent);
        EvtButtonReleaseId = EventManager::Subscribe(EventType::MouseButtonRelease, onEvent);
        EvtMouseMoveId = EventManager::Subscribe(EventType::MouseMove, onEvent);
        EvtMouseScrollId = EventManager::Subscribe(EventType::MouseScroll, onEvent);
    }

    void Human_View::OnDestroy()
    {
        EventManager::Unsubscribe(EventType::KeyPress, EvtKeyPressId);
        EventManager::Unsubscribe(EventType::KeyRelease, EvtKeyReleaseId);
        EventManager::Unsubscribe(EventType::MouseButtonPress, EvtButtonPressId);
        EventManager::Unsubscribe(EventType::MouseButtonRelease, EvtButtonReleaseId);
        EventManager::Unsubscribe(EventType::MouseMove, EvtMouseMoveId);
        EventManager::Unsubscribe(EventType::MouseScroll, EvtMouseScrollId);
    }

    void Human_View::Update(float elapsed_time)
    {
        for (auto itr = m_Layers.rbegin(); itr != m_Layers.rend(); itr++)
        {
            if ((*itr)->IsActive())
                (*itr)->Update(elapsed_time);
        }
    }

    void Human_View::Draw()
    {
        Graphics_Engine::BeforeDraw();

        for (auto &itr : m_Layers)
        {
            if (itr->IsActive())
                itr->Draw();
        }

        Graphics_Engine::AfterDraw();
    }

    void Human_View::PushLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.push_back(layer);
    }

    void Human_View::RemoveLayer(std::shared_ptr<Layer> layer)
    {
        m_Layers.remove(layer);
    }
}
