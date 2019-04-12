#pragma once
#include "PhysicsEngine.hpp"
#include "RigidBody.hpp"
#include "Actor.hpp"

#define PhysicsId 3

namespace Tarbora {
    class PhysicsComponent : public ActorComponent
    {
    public:
        PhysicsComponent() {}
        ~PhysicsComponent() {}

        ComponentId GetId() const { return PhysicsId; }

        bool Init(json data);
        void AfterInit();
        virtual void Update(float deltaTime) override;

        static ActorComponentPtr Creator() { return ActorComponentPtr(new PhysicsComponent()); }
    private:
        RigidBodyPtr m_Body;
    };
}
