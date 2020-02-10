#include "TransformComponent.hpp"
#include "PhysicsComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    TransformComponent::TransformComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_Transform = glm::mat4(1.f);
        m_Rotation = glm::vec3(0.f);
        m_Updated = false;
        m_Controller = nullptr;
        
        SetOrigin(table.Get<glm::vec3>("origin", true));
        SetPosition(table.Get<glm::vec3>("position"));
        SetRotation(table.Get<glm::vec3>("rotation"));
        SetRotation(table.Get<glm::vec3>("rotation"));
    }

    void TransformComponent::SetTransform(const glm::mat4 &transform)
    {
        m_Updated = true;
        m_Transform = transform;
    }

    void TransformComponent::SetPosition(const glm::vec3 &position)
    {
        m_Updated = true;
        Move(position - glm::vec3(m_Transform[3]));
    }

    void TransformComponent::SetRotation(const glm::vec3 &rotation)
    {
        m_Updated = true;
        Rotate(rotation - m_Rotation);
    }

    void TransformComponent::Move(const glm::vec3 &position)
    {
        m_Updated = true;
        m_Transform = glm::translate(m_Transform, position);
    }

    void TransformComponent::Rotate(const glm::vec3 &rotation)
    {
        m_Updated = true;
        glm::quat quat(glm::radians(rotation));
        m_Transform = glm::translate(m_Transform, m_Origin);
        m_Transform = m_Transform * glm::mat4_cast(quat);
        m_Transform = glm::translate(m_Transform, -m_Origin);
        m_Rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(m_Transform)));
    }

    void TransformComponent::SetOrigin(const glm::vec3 &origin)
    {
        m_Origin = origin;
    }

    glm::vec3 TransformComponent::GetPosition()
    {
        return glm::vec3(m_Transform[3]);
    }

    glm::vec3 TransformComponent::GetRotation()
    {
        return m_Rotation;
    }

    glm::mat3 TransformComponent::GetRotationMatrix()
    {
        return glm::mat3(m_Transform);
    }

    TransformSystem::TransformSystem(World *w) :
        SystemImpl<TransformComponent>(w)
    {
        Subscribe("set_position", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.id()));
            if (transform)
            {
                transform->SetPosition(m.magnitude() * Vec3toGLM(m.direction()));
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }

        });

        Subscribe("move", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.id()));
            if (transform)
            {
                transform->Move(m.magnitude() * Vec3toGLM(m.direction()));
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }
        });

        Subscribe("set_rotation", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.id()));
            if (transform)
            {
                transform->SetRotation(m.magnitude() * Vec3toGLM(m.direction()));
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }
        });

        Subscribe("rotate", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.id()));
            if (transform)
            {
                transform->Rotate(m.magnitude() * Vec3toGLM(m.direction()));
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }
        });
    }

    void TransformSystem::Update(float deltaTime)
    {
        for (auto &component : m_Components)
        {
            TransformComponent *transform = &component.second;
            if (transform->m_Updated)
            {
                Trigger("move_actor", MoveActor(
                    transform->m_Owner,
                    transform->GetPosition(),
                    transform->GetRotationMatrix()
                ));
                transform->m_Updated = false;
            }
        }
    }
}
