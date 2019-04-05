#include "Actor.hpp"
#include "Logger.hpp"
#include "Components.hpp"

namespace Tarbora {
    Actor::Actor(ActorId id)
    {
        m_Id = id;
    }

    Actor::~Actor()
    {
        Destroy();
    }

    bool Actor::Init(json data)
    {
        (void)(data);
        return true;
    }

    void Actor::AfterInit()
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->AfterInit();
        }
    }

    void Actor::Destroy()
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->Destroy();
            m_Components.erase(itr);
        }
    }

    void Actor::Update(float deltaTime)
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->Update(deltaTime);
        }
    }

    void Actor::AddComponent(ActorComponentPtr component)
    {
        m_Components.emplace(component->GetId(), component);
    }

    ActorComponentPtr Actor::GetComponent(ComponentId id)
    {
        auto itr = m_Components.find(id);
        if (itr != m_Components.end())
        {
            return itr->second;
        } else
        {
            return ActorComponentPtr();
        }
    }
}
