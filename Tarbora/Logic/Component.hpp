#pragma once
#include "System.hpp"

namespace Tarbora {
    class Component
    {
    public:
        Component(System *s, const ActorId &id, const LuaTable &table) :
            system_(s), owner_(id), enabled_(false), error_(false) { UNUSED(table); }

        inline Component* getComponent(const ActorId &id, const ComponentId &component)
        {
            return system_->getComponent(id, component);
        }

        inline Component* getComponent(const ComponentId &component)
        {
            return system_->getComponent(owner_, component);
        }

        void enable()
        {
            enabled_ = true;
            onEnable();
        }

        void disable()
        {
            enabled_ = false;
            onDisable();
        }

        inline void send(ClientId to, MessageSubject s, MessageBody b) const
        {
            system_->send(to, s, b);
        }

        inline void trigger(MessageSubject s, MessageBody b) const
        {
            system_->trigger(s, b);
        }

        inline void triggerLocal(MessageSubject s, MessageBody b) const
        {
            system_->triggerLocal(s, b);
        }

        bool error() { return error_; }

    protected:
        virtual void onEnable() {}

        virtual void onDisable() {}

        System *system_;
        ActorId owner_;
        bool enabled_;
        bool error_;
    };
}
