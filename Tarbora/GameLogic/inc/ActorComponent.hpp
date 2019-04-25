#pragma once
#include "../../Framework/Framework.hpp"

namespace Tarbora {
    class ActorFactory;
    class ActorTest;

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
        void SetOwner(ActorTest * owner) { m_Owner = owner; }

        ActorTest * m_Owner;
    };

    typedef std::shared_ptr<ActorComponent> ActorComponentPtr;
}
