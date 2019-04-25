#include "../inc/Actor.hpp"
#include "../inc/Components.hpp"

namespace Tarbora {
    ActorTest::ActorTest(ActorId id)
    {
        m_Id = id;
    }

    ActorTest::~ActorTest()
    {
        Destroy();
    }

    bool ActorTest::Init(json data)
    {
        (void)(data);
        return true;
    }

    void ActorTest::AfterInit()
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->AfterInit();
        }
    }

    void ActorTest::Destroy()
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->Destroy();
            m_Components.erase(itr);
        }
    }

    void ActorTest::Update(float deltaTime)
    {
        for (auto itr = m_Components.begin(); itr != m_Components.end(); itr++)
        {
            itr->second->Update(deltaTime);
        }
    }

    void ActorTest::AddComponent(ActorComponentPtr component)
    {
        m_Components.emplace(component->GetId(), component);
    }

    ActorComponentPtr ActorTest::GetComponent(ComponentId id)
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
