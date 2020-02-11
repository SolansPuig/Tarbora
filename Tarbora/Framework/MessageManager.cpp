#include "MessageManager.hpp"

namespace Tarbora {
    MessageManager::MessageManager(ClientId id)
    {
        m_MessageClient = std::unique_ptr<MessageClient>(new MessageClient(id));
        m_MessageClient->Connect();
    }

    MessageManager::~MessageManager()
    {
        m_MessageClient->Disconnect();
    }

    void MessageManager::ReadMessage(tbMessages::Message &m)
    {
        MessageBody b(m.body());

        auto allListeners = m_Listeners.find("all");
        if (allListeners != m_Listeners.end())
        {
            for (auto listener : allListeners->second)
            {
                listener(m.subject(), &b);
            }
        }

        auto subjectListeners = m_Listeners.find(m.subject());
        if (subjectListeners != m_Listeners.end())
        {
            for (auto listener : subjectListeners->second)
            {
                listener(m.subject(), &b);
            }
        }
    }

    void MessageManager::ReadMessages()
    {
        tbMessages::Message m;
        while (m_MessageClient->GetMessage(&m))
        {
            ReadMessage(m);
        }
    }

    SubscriptorId MessageManager::Subscribe(MessageSubject s, MessageFn func, bool local)
    {
        auto subjectListeners = m_Listeners.find(s);
        if (!local && subjectListeners == m_Listeners.end())
        {
            m_MessageClient->Subscribe(s);
        }

        m_Listeners[s].push_back(func);
        return m_Listeners[s].size() - 1;
    }

    void MessageManager::Desubscribe(MessageSubject s, SubscriptorId id)
    {
        m_Listeners[s].erase(m_Listeners[s].begin() + id);
        if (m_Listeners[s].size() == 0)
        {
            m_MessageClient->Desubscribe(s);
        }
    }

    void MessageManager::Trigger(MessageSubject s, MessageBody b)
    {
        tbMessages::Message message;
        message.set_type(tbMessages::MessageType::EVENT);
        message.set_subject(s);
        message.set_body(b.GetContent());

        m_MessageClient->Send(message);
    }

    void MessageManager::Send(ClientId to, MessageSubject s, MessageBody b)
    {
        tbMessages::Message message;
        message.set_type(tbMessages::MessageType::COMMAND);
        message.set_to(to);
        message.set_subject(s);
        message.set_body(b.GetContent());

        m_MessageClient->Send(message);
    }

    void MessageManager::TriggerLocal(MessageSubject s, MessageBody b)
    {
        tbMessages::Message message;
        message.set_type(tbMessages::MessageType::LOCAL);
        message.set_subject(s);
        message.set_body(b.GetContent());

        ReadMessage(message);
    }
}
