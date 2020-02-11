#include "TransformComponent.hpp"
#include "PhysicsComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    TransformComponent::TransformComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_Transform = glm::mat4(1.f);
        m_Rotation = glm::vec3(0.f);
        m_ShouldUpdate = false;
        m_Controller = nullptr;

        SetOrigin(table.Get<glm::vec3>("origin", true));
        SetPosition(table.Get<glm::vec3>("position"));
        SetRotation(table.Get<glm::vec3>("rotation"));
        SetRotation(table.Get<glm::vec3>("rotation"));
    }

    void TransformComponent::SetTransform(const glm::mat4 &transform)
    {
        m_ShouldUpdate = true;
        m_Transform = transform;
    }

    void TransformComponent::SetPosition(const glm::vec3 &position)
    {
        m_ShouldUpdate = true;
        Move(position - glm::vec3(m_Transform[3]));
    }

    void TransformComponent::SetRotation(const glm::vec3 &rotation)
    {
        m_ShouldUpdate = true;
        Rotate(rotation - m_Rotation);
    }

    void TransformComponent::Move(const glm::vec3 &position)
    {
        m_ShouldUpdate = true;
        m_Transform = glm::translate(m_Transform, position);
    }

    void TransformComponent::Rotate(const glm::vec3 &rotation)
    {
        m_ShouldUpdate = true;
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
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.GetId()));
            if (transform)
            {
                transform->SetPosition(m.GetMagnitude() * m.GetDirection());
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }

        });

        Subscribe("move", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.GetId()));
            if (transform)
            {
                transform->Move(m.GetMagnitude() * m.GetDirection());
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }
        });

        Subscribe("set_rotation", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.GetId()));
            if (transform)
            {
                transform->SetRotation(m.GetMagnitude() * m.GetDirection());
                if (transform->m_Controller)
                    transform->m_Controller->UpdateTransform();
            }
        });

        Subscribe("rotate", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(Get(m.GetId()));
            if (transform)
            {
                transform->Rotate(m.GetMagnitude() * m.GetDirection());
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
            if (transform->m_ShouldUpdate)
            {
                Trigger("move_actor", Message::MoveActor(
                    transform->m_Owner,
                    Position(transform->GetPosition()),
                    RotationMat(transform->GetRotationMatrix())
                ));
                transform->m_ShouldUpdate = false;
            }
        }
    }
}
