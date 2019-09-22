#pragma once

#include "MessageClient.hpp"

namespace Tarbora {
    class Module;

    typedef unsigned int SubscriptorId;

    class MessageBody
    {
    public:
        MessageBody(std::string body)
            : m_Body(body) {}

        MessageBody() {}

        std::string GetContentStr() const
        {
            return m_Body;
        }

        template <class T>
        T GetContent() const
        {
            T m;
            m.ParseFromString(m_Body);
            return m;
        }

    protected:
        std::string m_Body;
    };

    typedef std::function<void(MessageSubject, MessageBody*)> MessageFn;

    class MessageManager
    {
    public:
        MessageManager(Module *m, ClientId id, std::string serverAddress);
        ~MessageManager();

        void ReadMessages();

        SubscriptorId Subscribe(MessageSubject s, MessageFn func);
        void Desubscribe(MessageSubject s, SubscriptorId id);

        void Trigger(MessageSubject s, MessageBody b);
        void Send(ClientId to, MessageSubject s, MessageBody b);

    private:
        Module *m_Module;
        std::unique_ptr<MessageClient> m_MessageClient;

        std::map<std::string, std::vector<MessageFn>> m_Listeners;
    };
}
