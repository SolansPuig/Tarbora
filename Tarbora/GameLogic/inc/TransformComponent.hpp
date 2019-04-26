#pragma once
#include "Actor.hpp"

#define TransformId 0

namespace Tarbora {
    class TransformComponent : public ActorComponent
    {
    public:
        TransformComponent() {}
        ~TransformComponent() {}

        ComponentId GetId() const { return TransformId; }

        bool Init(JsonPtr resource, json data)
        {
            m_Transform = glm::mat4(1.0f);
            m_Transform = glm::translate(m_Transform, glm::vec3(resource->GetFloatArray(data, "position", 0), resource->GetFloatArray(data, "position", 1), resource->GetFloatArray(data, "position", 2)));
            m_Transform = glm::rotate(m_Transform, glm::radians(resource->GetFloatArray(data, "rotation", 0)), glm::vec3(1.f, 0.f, 0.f));
            m_Transform = glm::rotate(m_Transform, glm::radians(resource->GetFloatArray(data, "rotation", 1)), glm::vec3(0.f, 1.f, 0.f));
            m_Transform = glm::rotate(m_Transform, glm::radians(resource->GetFloatArray(data, "rotation", 2)), glm::vec3(0.f, 0.f, 1.f));
            return true;
        }

        glm::mat4 &GetTransform() { return m_Transform; }
        void SetTransform(glm::mat4 transform) { m_Transform = transform; }

        glm::vec3 GetPosition()
        {
            return glm::vec3(m_Transform[3]);
        }

        glm::mat3 GetRotation()
        {
            return glm::mat3(m_Transform);
        }

        static ActorComponentPtr Creator() { return ActorComponentPtr(new TransformComponent()); }
    private:
        glm::mat4 m_Transform;
    };
}
