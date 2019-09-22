#pragma once
#include "../../Framework/ModuleComponent.hpp"
#include "World.hpp"

namespace Tarbora {
    class ActorFactory;
    class ActorTest;

    class ActorComponent : public ModuleComponent
    {
        friend class ActorFactory;
    public:
        ActorComponent(World *world) : ModuleComponent(world) {}
        virtual ~ActorComponent() {}

        virtual bool Init(JsonPtr resource, raw_json data) = 0;
        virtual void Destroy() {}
        virtual void AfterInit() {}
        virtual void Update(float deltaTime) { (void)(deltaTime); }

        virtual ComponentId GetId() const = 0;

    protected:
        void SetOwner(ActorTest * owner) { m_Owner = owner; }

        ActorTest * m_Owner;
    };

    typedef std::shared_ptr<ActorComponent> ActorComponentPtr;
}
