#pragma once
#include "ActorComponent.hpp"
#include <map>

#define INVALID_ID 0
#define MAIN_CAMERA_ID 15
#define SKY_ID 10

namespace Tarbora {
    typedef unsigned int ActorId;
    class Actors;
    class TypeComponent;

    class Actor
    {
        friend class ActorFactory;
        friend class Actors;
        typedef std::map<ComponentId, ActorComponentPtr> ActorComponents;
    public:
        explicit Actor(ActorId id);
        ~Actor();

        bool Init(json data);
        void AfterInit();
        void Destroy();
        void Update(float deltaTime);

        ActorId GetId() const { return m_Id; }

        ActorComponentPtr GetComponent(ComponentId id);

        Actor *GetNext() const { return m_Next; }
        void SetNext(Actor *next) { m_Next = next; }

    private:
        void AddComponent(ActorComponentPtr component);

        ActorId m_Id;

        ActorComponents m_Components;
        Actor *m_Next;
    };
}
