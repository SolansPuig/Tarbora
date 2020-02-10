#pragma once
#include "System.hpp"

namespace Tarbora {
    class Component
    {
    public:
        Component(System *s, const ActorId &id, const LuaTable &table) :
            m_System(s), m_Owner(id), m_Enabled(false), m_Error(false) {}

        inline Component *GetComponent(const ActorId &id, const ComponentId &compId)
        {
            return m_System->GetComponent(id, compId);
        }

        inline Component *GetComponent(const ComponentId &compId)
        {
            return m_System->GetComponent(m_Owner, compId);
        }

        void Enable()
        {
            m_Enabled = true;
            OnEnable();
        }

        void Disable()
        {
            m_Enabled = false;
            OnDisable();
        }

        inline void Send(ClientId to, MessageSubject s, MessageBody b) const
        {
            m_System->Send(to, s, b);
        }

        inline void Trigger(MessageSubject s, MessageBody b) const
        {
            m_System->Trigger(s, b);
        }

        inline void TriggerLocal(MessageSubject s, MessageBody b) const
        {
            m_System->TriggerLocal(s, b);
        }

        bool Error() { return m_Error; }

    protected:
        virtual void OnEnable() {}

        virtual void OnDisable() {}

        System *m_System;
        ActorId m_Owner;
        bool m_Enabled;
        bool m_Error;
    };
}
