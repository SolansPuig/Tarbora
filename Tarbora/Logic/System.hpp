#pragma once
#include "../Framework/ModuleComponent.hpp"
#include "World.hpp"

namespace Tarbora {
    class Component;

    class System : public ModuleComponent
    {
    public:
        System(World *w) : ModuleComponent(w), m_World(w) {}

        virtual void Add(const ActorId &id, const LuaTable &table) = 0;
        virtual Component *Get(const ActorId &id) = 0;
        inline Component *GetComponent(const ActorId &id, const ComponentId &compId) { return m_World->GetComponent(id, compId); }
        virtual void Init(const ActorId &id) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Enable(const ActorId &id) = 0;
        virtual void Disable(const ActorId &id) = 0;

    protected:
        World *m_World;
    };

    template <class T>
    class SystemImpl : public System
    {
    public:
        SystemImpl(World *w) : System(w) {}

        virtual void Add(const ActorId &id, const LuaTable &table)
        {
            m_Components.emplace(id, T(this, id, table));
        }

        virtual Component *Get(const ActorId &id)
        {
            auto component = m_Components.find(id);
            if (component != m_Components.end()) {
                return &component->second;
            }
            return nullptr;
        }

        virtual void Init(const ActorId &id)
        {
            T *component =  static_cast<T*>(Get(id));
            if (component && !component->Error())
                component->Enable();
        }

        virtual void Update(float deltaTime) {}

        virtual void Enable(const ActorId &id)
        {
            auto component = m_Components.find(id);
            if (component != m_Components.end()) {
                component->second.Enable();
            }
        }

        virtual void Disable(const ActorId &id)
        {
            auto component = m_Components.find(id);
            if (component != m_Components.end()) {
                component->second.Disable();
            }
        }

    protected:
        std::map<ActorId, T> m_Components;
    };
}
