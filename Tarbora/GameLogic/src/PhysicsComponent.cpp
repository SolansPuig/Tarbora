#include "../inc/PhysicsComponent.hpp"
#include "../inc/TransformComponent.hpp"

namespace Tarbora {
    bool PhysicsComponent::Init(json data)
    {
        std::string shape = data["shape"];
        if (shape == "sphere")
        {
            m_Body.reset(new SphereBody(data["radius"]));
            m_Body->SetProperties(data["friction"], data["density"], data["restitution"]);
            return true;
        }
        else if (shape == "box")
        {
            glm::vec3 dimensions = glm::vec3(data["size"][0], data["size"][1], data["size"][2]);
            m_Body.reset(new BoxBody(dimensions));
            m_Body->SetProperties(data["friction"], data["density"], data["restitution"]);
            return true;
        }

        return false;
    }

    void PhysicsComponent::AfterInit()
    {
        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            glm::mat4 transposed = transform->GetTransform();
            m_Body->Register(m_Owner->GetId(), transposed);
        }

        EventManager::Subscribe(EventType::ApplyForce, [&](Event *ev)
        {
            ApplyForceEvent *e = static_cast<ApplyForceEvent*>(ev);
            if (e->actorId == m_Owner->GetId())
            {
                m_Body->ApplyForce(e->newtons, e->direction);
            }
        });

        EventManager::Subscribe(EventType::ApplyTorque, [&](Event *ev)
        {
            ApplyTorqueEvent *e = static_cast<ApplyTorqueEvent*>(ev);
            if (e->actorId == m_Owner->GetId())
            {
                m_Body->ApplyTorque(e->magnitude, e->direction);
            }
        });

        EventManager::Subscribe(EventType::SetVelocity, [&](Event *ev)
        {
            SetVelocityEvent *e = static_cast<SetVelocityEvent*>(ev);
            if (e->actorId == m_Owner->GetId())
                m_Body->SetVelocity(e->direction);
        });

        EventManager::Subscribe(EventType::Stop, [&](Event *ev)
        {
            StopEvent *e = static_cast<StopEvent*>(ev);
            if (e->actorId == m_Owner->GetId())
                m_Body->Stop();
        });
    }

    void PhysicsComponent::Update(float deltaTime)
    {
        ActorId id = m_Owner->GetId();
        ActorMotionState *motionState = static_cast<ActorMotionState*>(m_Body->Get()->getMotionState());

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));

        if (motionState)
        {
            if (transform)
            {
                if (transform->GetTransform() != motionState->m_Transform)
                {
                    transform->SetTransform(motionState->m_Transform);
                    ActorMoveEvent ev(id, motionState->getPosition(), motionState->getRotation());
                    EventManager::Trigger(EventType::ActorMove, &ev);
                }
            }
        }
    }
}
