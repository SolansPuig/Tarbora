#pragma once
#include "Actor.hpp"
#include "../../Framework/PhysicsEngine/inc/RigidBody.hpp"

#define CharacterControllerId 4

namespace Tarbora {
    class CharacterControllerComponent : public ActorComponent
    {
    public:
        CharacterControllerComponent() {}
        ~CharacterControllerComponent() {}

        ComponentId GetId() const { return CharacterControllerId; }

        bool Init(JsonPtr resource, raw_json data);
        void AfterInit();
        virtual void Update(float deltaTime) override;

        static ActorComponentPtr Creator() { return ActorComponentPtr(new CharacterControllerComponent()); }
    private:
        RigidBodyPtr m_Body;
        glm::vec3 m_Movement;
        bool m_OnGround;
        bool m_Flying;
        float m_Height;
        float m_Width;
        float m_StepHeight;
    };
}
