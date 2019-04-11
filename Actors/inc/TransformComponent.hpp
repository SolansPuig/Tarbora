#pragma once
#include "Actor.hpp"

#define TransformId 3

namespace Tarbora {
    class TransformComponent : public ActorComponent
    {
    public:
        TransformComponent() {}
        ~TransformComponent() {}

        ComponentId GetId() const { return TransformId; }

        bool Init(json data) { m_Transform = glm::mat4(1.0f); }

        glm::mat4 &GetTransform() { return m_Transform; }
        void SetTransform(glm::mat4 transform) { m_Transform = transform; }

        static ActorComponentPtr Creator() { return ActorComponentPtr(new TransformComponent()); }
    private:
        glm::mat4 m_Transform;
    };
}
