#include "Actors.hpp"
#include "Components.hpp"
#include "Events.hpp"

namespace Tarbora {
    Actors::Actors() {}

    Actors::~Actors() {}

    void Actors::Init(ActorId maxNumber)
    {
        m_ActorList.reserve(maxNumber);
        m_FirstAvailable = &m_ActorList[1]; // 0 is the invalid id

        for (ActorId id = 0; id < maxNumber; id++)
        {
            m_ActorList.push_back(Actor(id));
            if (id+1 < maxNumber) m_ActorList[id].SetNext(&m_ActorList[id+1]);
            else m_ActorList[id].SetNext(nullptr);
        }

        AddComponentCreator("type", TypeComponent::Creator);
        AddComponentCreator("model", ModelComponent::Creator);

        EventManager::Subscribe(EventType::CreateActor, [&](Event *e)
        {
            CreateActorEvent *ev = static_cast<CreateActorEvent*>(e);
            Create(ev->entity);
        });
    }

    ActorId Actors::Create(std::string entity)
    {
        if (m_FirstAvailable != nullptr)
        {
            Actor *actor = m_FirstAvailable;
            if (m_ActorFactory.Create(actor, entity))
            {
                LOG_DEBUG("Create actor of entity %s with id %d", entity.c_str(), actor->GetId());
                m_FirstAvailable = actor->GetNext();
                actor->SetNext(nullptr);
                return actor->GetId();
            }
            LOG_ERR("Actors: Could not create actor.");
            return 0;
        }
        LOG_ERR("Actors: There's no left space in the actor pool.");
        return 0;
    }

    void Actors::Update(float deltaTime)
    {
        for (auto itr = m_ActorList.begin(); itr != m_ActorList.end(); itr++)
        {
            if (itr->GetNext() == nullptr)
            {
                itr->Update(deltaTime);
            }
        }
    }

    void Actors::Destroy(ActorId id)
    {
        m_ActorList[id].Destroy();
        m_ActorList[id].SetNext(m_FirstAvailable);
        m_FirstAvailable = &m_ActorList[id];
    }

    ActorPtr Actors::Get(ActorId id)
    {
        return ActorPtr(&m_ActorList[id]);
    }

    ActorComponentPtr Actors::GetComponent(ActorId id, ComponentId compId)
    {
        return ActorComponentPtr(m_ActorList[id].GetComponent(compId));
    }

    void Actors::AddComponentCreator(std::string name, ActorComponentCreator func)
    {
        m_ActorFactory.AddComponentCreator(name, func);
    }
}
