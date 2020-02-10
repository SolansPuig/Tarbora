#pragma once
#include "Component.hpp"
#include "../Framework/PhysicsEngine/inc/RigidBody.hpp"

namespace Tarbora {
    class TransformComponent;

    class PhysicsComponent : public Component
    {
        friend class PhysicsSystem;
    public:
        PhysicsComponent(System *s, const ActorId &id, const LuaTable &table);

        void UpdateTransform();

        void RestrictRotation(const glm::vec3 &rotation);

        void SetLinearDamping(float value);

        void SetVelocity(float magnitude, const glm::vec3 &direction);

        void SetAngularVelocity(float magnitude, const glm::vec3 &direction);

        void ApplyForce(float magnitude, const glm::vec3 &direction);

        void ApplyTorque(float magnitude, const glm::vec3 &direction);

        void Stop();

        std::shared_ptr<RayCastResult> RayCast(const glm::vec3 &origin, const glm::vec3 &direction, float length);

        float GetHeight() { return m_Height; }



    private:
        virtual void OnEnable();
        virtual void OnDisable();

        std::shared_ptr<RigidBody> m_Body;
        TransformComponent *m_Transform;
        ActorMotionState *m_MotionState;
        float m_Height;
    };

    class PhysicsSystem : public SystemImpl<PhysicsComponent>
    {
    public:
        PhysicsSystem(World *w);
        ~PhysicsSystem();

        static std::string GetName() { return "physics"; }

        virtual void Init(const ActorId &id);

        virtual void Update(float deltaTime);
    };
}
