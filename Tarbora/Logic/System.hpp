#pragma once
#include "World.hpp"

namespace Tarbora {
    class Component;

    class System : public ModuleComponent
    {
    public:
        System(World *w) : ModuleComponent(w), world_(w) {}

        virtual void add(const ActorId &id, const LuaTable &table) = 0;
        virtual Component* get(const ActorId &id) = 0;
        inline Component* getComponent(const ActorId &id, const ComponentId &component) { return world_->getComponent(id, component); }
        virtual void init(const ActorId &id) = 0;
        virtual void update(float delta_time) = 0;
        virtual void enable(const ActorId &id) = 0;
        virtual void disable(const ActorId &id) = 0;

    protected:
        World *world_;
    };

    template <class T>
    class SystemImpl : public System
    {
    public:
        SystemImpl(World *w) : System(w) {}

        virtual void add(const ActorId &id, const LuaTable &table)
        {
            components_.emplace(id, T(this, id, table));
        }

        virtual Component* get(const ActorId &id)
        {
            auto component = components_.find(id);
            if (component != components_.end()) {
                return &component->second;
            }
            return nullptr;
        }

        virtual void init(const ActorId &id)
        {
            T *component =  static_cast<T*>(get(id));
            if (component && !component->error())
                component->enable();
        }

        virtual void update(float delta_time) { UNUSED(delta_time); }

        virtual void enable(const ActorId &id)
        {
            auto component = components_.find(id);
            if (component != components_.end()) {
                component->second.enable();
            }
        }

        virtual void disable(const ActorId &id)
        {
            auto component = components_.find(id);
            if (component != components_.end()) {
                component->second.disable();
            }
        }

    protected:
        std::map<ActorId, T> components_;
    };
}
