#pragma once
#include "Actor.hpp"
#include "../../Framework/PhysicsEngine/inc/RigidBody.hpp"

#define PhysicsId 3

namespace Tarbora {
    class PhysicsComponent : public ActorComponent
    {
    public:
        PhysicsComponent() {}
        ~PhysicsComponent() {}

        ComponentId GetId() const { return PhysicsId; }

        bool Init(JsonPtr resource, raw_json data);
        void AfterInit();
        virtual void Update(float deltaTime) override;

        static ActorComponentPtr Creator() { return ActorComponentPtr(new PhysicsComponent()); }
    private:
        RigidBodyPtr m_Body;
    };
}