#pragma once
#include <memory>
#include "Resource.hpp"

namespace Tarbora {
    class ActorFactory;
    class Actor;
    typedef std::shared_ptr<Actor> ActorPtr;
    typedef unsigned int ComponentId;

    class ActorComponent
    {
        friend class ActorFactory;
    public:
        virtual ~ActorComponent() {}

        virtual bool Init(json data) = 0;
        virtual void Destroy() {}
        virtual void AfterInit() {}
        virtual void Update(float deltaTime) { (void)(deltaTime); }

        virtual ComponentId GetId() const = 0;

    protected:
        void SetOwner(Actor *owner) { m_Owner = owner; }

        Actor *m_Owner;
    };

    typedef std::shared_ptr<ActorComponent> ActorComponentPtr;
}
