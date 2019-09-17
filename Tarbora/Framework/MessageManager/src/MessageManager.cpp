#include "../inc/MessageManager.hpp"

namespace Tarbora {
    MessageManager::MessageManager(Module *m, ClientId id, std::string serverAddress) :
        m_Module(m)
    {
        m_NetworkClient = std::unique_ptr<NetworkClient>(new NetworkClient(id, serverAddress));
        m_NetworkClient->Connect();
    }

    MessageManager::~MessageManager()
    {
        m_NetworkClient->Disconnect();
    }

    void MessageManager::ReadMessages()
    {
        Message m;
        while (m_NetworkClient->GetMessage(&m))
        {
            auto all = m_Listeners.find("all");
            if (all != m_Listeners.end())
            {
                for (auto listener : all->second)
                {
                    listener(m.subject(), m.body());
                }
            }

            auto type = m_Listeners.find(m.subject());
            if (type != m_Listeners.end())
            {
                for (auto listener : type->second)
                {
                    listener(m.subject(), m.body());
                }
            }
        }
    }

    EventId MessageManager::Subscribe(std::string type, EventFn func)
    {
        auto typeListeners = m_Listeners.find(type);
        if (typeListeners == m_Listeners.end())
        {
            m_NetworkClient->Subscribe(type);
        }

        m_Listeners[type].push_back(func);
        return m_Listeners[type].size() - 1;
    }

    void MessageManager::Desubscribe(std::string type, EventId id)
    {
        m_Listeners[type].erase(m_Listeners[type].begin() + id);
        if (m_Listeners[type].size() == 0)
        {
            m_NetworkClient->Desubscribe(type);
        }
    }

    void MessageManager::Trigger(std::string type, MessageBody body)
    {
        Message message;
        message.set_type(MessageType::EVENT);
        message.set_subject(type);
        message.set_body(body);

        m_NetworkClient->Send(message);
    }

     void MessageManager::Send(ClientId to, std::string type, MessageBody body)
    {
        Message message;
        message.set_type(MessageType::COMMAND);
        message.set_to(to);
        message.set_subject(type);
        message.set_body(body);

        m_NetworkClient->Send(message);
    }
}
