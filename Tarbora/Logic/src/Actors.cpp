#include "../inc/Actors.hpp"
#include "../inc/Components.hpp"
#include "../inc/TransformComponent.hpp"
#include "../inc/PhysicsComponent.hpp"
#include "../inc/CharacterControllerComponent.hpp"

namespace Tarbora {
    Actors::Actors(std::string server_address, ActorId maxNumber) :
        ClientApplication(1, server_address)
    {
        PhysicsEngine::Init();
        m_ActorFactory = std::make_unique<ActorFactory>(this);
        m_ActorList.reserve(maxNumber);
        m_FirstAvailable = &m_ActorList[1]; // 0 is the invalid id

        for (ActorId id = 0; id < maxNumber; id++)
        {
            m_ActorList.push_back(ActorTest(id));
            if (id+1 < maxNumber) m_ActorList[id].SetNext(&m_ActorList[id+1]);
            else m_ActorList[id].SetNext(nullptr);
        }

        AddComponentCreator("type", TypeComponent::Creator);
        AddComponentCreator("model", ModelComponent::Creator);
        AddComponentCreator("transform", TransformComponent::Creator);
        AddComponentCreator("physics", PhysicsComponent::Creator);
        AddComponentCreator("character_controller", CharacterControllerComponent::Creator);

        MessageManager()->Subscribe("create_actor", [this](std::string subject, std::string body)
        {
            CreateActorMessage m;
            m.ParseFromString(body);

            Create(m.entity(), Vec3toGLM(m.position()), Vec3toGLM(m.rotation()));
        });
    }

    Actors::~Actors()
    {
        for (ActorId id = 1; id <= m_ActorList.size(); id++)
        {
            Destroy(id);
        }
        PhysicsEngine::Close();
    }

    ActorId Actors::Create(std::string entity, glm::vec3 initialPos, glm::vec3 initalRot)
    {
        if (m_FirstAvailable != nullptr)
        {
            ActorTest *actor = m_FirstAvailable;
            if (m_ActorFactory->Create(actor, entity, initialPos, initalRot))
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
        PhysicsEngine::Update(deltaTime);
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
        m_ActorList[id-1].Destroy();
        m_ActorList[id-1].SetNext(m_FirstAvailable);
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
        m_ActorFactory->AddComponentCreator(name, func);
    }
}
