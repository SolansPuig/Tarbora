#include "Scene.hpp"
#include <utility>
#include "Logger.hpp"
#include "Events.hpp"
#include "Graphics_Engine.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {
    Scene::Scene()
    {
        m_Root.reset(new RootNode());
        m_Camera.reset(new Camera(MAIN_CAMERA_ID, "body"));
        AddChild(m_Camera);
        m_Projection = glm::perspective(glm::radians(45.0f), Graphics_Engine::GetWindow()->GetRatio(), 0.1f, 100.0f);

        EventFn changeProjection = [this](Event* e)
        {
            (void)(e);
            m_Projection = glm::perspective(glm::radians(45.0f), Graphics_Engine::GetWindow()->GetRatio(), 0.1f, 100.0f);
        };

        EvtWindowResizeId = EventManager::Subscribe(EventType::WindowResize, changeProjection);

        EventFn onActorEvent = [this](Event* e)
        {
            ActorEvent *ev = static_cast<ActorEvent*>(e);
            if (!m_ActorMap[ev->actorId]->OnActorEvent(ev))
                LOG_WARN("Scene: Target %u has no child named %s to parse event of type %d", ev->actorId, ev->name.c_str(), e->GetType());
        };
        EvtActorMoveId = EventManager::Subscribe(EventType::ActorMove, onActorEvent);
        EvtActorRotateId = EventManager::Subscribe(EventType::ActorRotate, onActorEvent);
    }

    Scene::~Scene()
    {
        EventManager::Unsubscribe(EventType::WindowResize, EvtWindowResizeId);
        EventManager::Unsubscribe(EventType::ActorMove, EvtActorMoveId);
        EventManager::Unsubscribe(EventType::ActorRotate, EvtActorRotateId);
    }

    void Scene::Update(float deltaTime)
    {
        if (m_Root)
        {
            m_Root->Update(deltaTime);
        }
    }

    void Scene::Draw()
    {
        if (m_Root)
        {
            glm::mat4 view = m_Camera->GetView();
            glm::mat4 matrix = m_Projection * view;
            m_Root->DrawChildren(this, &matrix);
        }
    }

    void Scene::AddChild(SceneNodePtr child)
    {
        ActorId id = child->GetActorId();
        if (id != INVALID_ID)
            m_ActorMap[id] = child;
        m_Root->AddChild(child);
    }

    SceneNodePtr Scene::GetChild(ActorId id)
    {
        auto itr = m_ActorMap.find(id);
        if (itr == m_ActorMap.end())
        {
            return SceneNodePtr();
        }
        return itr->second;
    }

    bool Scene::RemoveChild(ActorId id)
    {
        if (id == INVALID_ID)
            return false;
        SceneNodePtr child = GetChild(id);
        m_ActorMap.erase(id);
        return m_Root->RemoveChild(id);
    }
}
