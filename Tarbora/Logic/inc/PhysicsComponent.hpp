#pragma once
#include "Actor.hpp"
#include "../../Framework/PhysicsEngine/inc/RigidBody.hpp"

#define PhysicsId 3

namespace Tarbora {
    class PhysicsComponent : public ActorComponent
    {
    public:
        PhysicsComponent(World *world) : ActorComponent(world) {}
        ~PhysicsComponent() {}

        ComponentId GetId() const { return PhysicsId; }

        bool Init(JsonPtr resource, raw_json data);
        void AfterInit();
        virtual void Update(float deltaTime) override;

        static ActorComponentPtr Creator(World *world) { return ActorComponentPtr(new PhysicsComponent(world)); }
    private:
        RigidBodyPtr m_Body;
    };
}
