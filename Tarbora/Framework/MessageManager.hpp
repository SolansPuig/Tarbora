#pragma once

#include "MessageClient.hpp"

namespace Tarbora {
    typedef unsigned int SubscriptorId;

    class MessageBody
    {
    public:
        MessageBody(const std::string &body)
            : m_Body(body) {}

        MessageBody() {}

        const std::string &GetContent() const
        {
            return m_Body;
        }

    protected:
        std::string m_Body;
    };

    typedef std::function<void(MessageSubject, MessageBody*)> MessageFn;

    class MessageManager
    {
    public:
        MessageManager(ClientId id);
        ~MessageManager();

        inline void SetDebugName(std::string name="Unamed Message Client")
        {
            m_MessageClient->SetDebugName(name);
        }

        void ReadMessages();

        SubscriptorId Subscribe(MessageSubject s, MessageFn func, bool local=false);
        void Desubscribe(MessageSubject s, SubscriptorId id);

        void Trigger(MessageSubject s, MessageBody b);
        void Send(ClientId to, MessageSubject s, MessageBody b);
        void TriggerLocal(MessageSubject s, MessageBody b);

    private:
        void ReadMessage(tbMessages::Message &m);

        std::unique_ptr<MessageClient> m_MessageClient;

        std::map<std::string, std::vector<MessageFn>> m_Listeners;
    };
}
