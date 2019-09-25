#pragma once
#include "Actor.hpp"
#include "../../Framework/PhysicsEngine/inc/RigidBody.hpp"

#define CharacterControllerId 4

namespace Tarbora {
    class CharacterControllerComponent : public ActorComponent
    {
    public:
        CharacterControllerComponent(World *world) : ActorComponent(world) {}
        ~CharacterControllerComponent() {}

        ComponentId GetId() const { return CharacterControllerId; }

        bool Init(JsonPtr resource, raw_json data);
        void AfterInit();
        virtual void Update(float deltaTime) override;

        static ActorComponentPtr Creator(World *world) { return ActorComponentPtr(new CharacterControllerComponent(world)); }
    private:
        RigidBodyPtr m_Body;
        glm::vec3 m_Movement;
        glm::vec3 m_Rotation;
        glm::vec3 m_LookRotation;
        bool m_OnGround;
        bool m_Flying;
        float m_Height;
        float m_Width;
        float m_Speed;
        float m_RunSpeed;

        void CheckLineOfSight();
        void CheckGround();
        void UpdateTransformAndView();
    };
}
