#include "PhysicsComponent.hpp"
#include "TransformComponent.hpp"
#include "Events.hpp"
#include <math.h>
#include <glm/gtx/string_cast.hpp>

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
