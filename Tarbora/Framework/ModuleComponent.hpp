#pragma once

#include "Module.hpp"

namespace Tarbora {
    class ModuleComponent
    {
    public:
        ModuleComponent(Module *m) : m_Module(m) {}
        ~ModuleComponent();

        virtual void Update(float elapsedTime) {};
        virtual void Draw() {};

        void Subscribe(MessageSubject s, MessageFn callback);
        void Desubscribe(MessageSubject s);

        inline void Send(ClientId to, MessageSubject s, MessageBody b) const
        {
            m_Module->GetMessageManager()->Send(to, s, b);
        }

        inline void Trigger(MessageSubject s, MessageBody b) const
        {
            m_Module->GetMessageManager()->Trigger(s, b);
        }

        inline void TriggerLocal(MessageSubject s, MessageBody b) const
        {
            m_Module->GetMessageManager()->TriggerLocal(s, b);
        }

    protected:
        Module *m_Module;

    private:
        std::unordered_map<std::string, SubscriptorId> m_SubscribedMessages;

        inline void Desubscribe(MessageSubject s, SubscriptorId id)
        {
            m_Module->GetMessageManager()->Desubscribe(s, id);
        }
    };
}
