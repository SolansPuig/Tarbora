#pragma once
#include "Component.hpp"
#include "../Framework/PhysicsEngine/inc/RigidBody.hpp"

namespace Tarbora {
    class PhysicsComponent;
    class AnimationComponent;

    class ControllerComponent : public Component
    {
        friend class ControllerSystem;
    public:
        ControllerComponent(System *s, const ActorId &id, const LuaTable &table);

        void SetMovement(const glm::vec3 &direction);

        void SetLookDirection(const glm::vec3 &direction);

    private:
        void CheckGround();

        PhysicsComponent *m_Physics;
        AnimationComponent *m_Animation;
        float m_Speed;
        float m_RunSpeed;
        glm::vec3 m_Movement;
        glm::vec3 m_Rotation;
        glm::vec3 m_LookRotation;
        bool m_OnGround;
        bool m_Walking;
        bool m_ShouldUpdate;
    };

    class ControllerSystem : public SystemImpl<ControllerComponent>
    {
    public:
        ControllerSystem(World *w);

        static std::string GetName() { return "controller"; }

        virtual void Init(const ActorId &id);

        virtual void Update(float deltaTime);
    };
}
