#pragma once
#include "ActorComponent.hpp"

namespace Tarbora {
    class World;

    class ActorTest
    {
        friend class ActorFactory;
        friend class World;
        typedef std::map<ComponentId, ActorComponentPtr> ActorComponents;
    public:
        ActorTest(ActorId id);
        ~ActorTest();

        bool Init(std::string name);
        void AfterInit();
        void Destroy();
        void Update(float deltaTime);

        ActorId GetId() const { return m_Id; }
        std::string GetEntityName() const { return m_EntityName; }

        ActorComponentPtr GetComponent(ComponentId id);

        ActorTest *GetNext() const { return m_Next; }
        void SetNext(ActorTest *next) { m_Next = next; }

    private:
        void AddComponent(ActorComponentPtr component);

        ActorId m_Id;
        std::string m_EntityName;

        ActorComponents m_Components;
        ActorTest *m_Next;
    };

    typedef std::shared_ptr<ActorTest> ActorPtr;
}
