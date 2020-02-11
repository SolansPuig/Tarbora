#pragma once
#include "Component.hpp"

namespace Tarbora {
    class PhysicsComponent;

    class TransformComponent : public Component
    {
        friend class TransformSystem;
    public:
        TransformComponent(System *s, const ActorId &id, const LuaTable &table);

        void SetController(PhysicsComponent *controller) { m_Controller = controller; }

        void SetTransform(const glm::mat4 &transform);

        void SetPosition(const glm::vec3 &position);

        void SetRotation(const glm::vec3 &rotation);

        void Move(const glm::vec3 &position);

        void Rotate(const glm::vec3 &rotation);

        void SetOrigin(const glm::vec3 &origin);

        const glm::mat4 &GetTransform() { return m_Transform; }

        glm::vec3 GetPosition();

        glm::vec3 GetRotation();

        glm::vec3 GetOrigin();

        glm::mat3 GetRotationMatrix();

    private:
        glm::mat4 m_Transform;
        glm::vec3 m_Rotation;
        glm::vec3 m_Origin;
        PhysicsComponent *m_Controller;
        bool m_ShouldUpdate;
    };

    class TransformSystem : public SystemImpl<TransformComponent>
    {
    public:
        TransformSystem(World *w);

        static std::string GetName() { return "transform"; }

        virtual void Update(float deltaTime);
    };
}
