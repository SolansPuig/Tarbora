#include "MessageHub.hpp"

using tbMessages::Message;
using tbMessages::TarboraMessages;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;

namespace Tarbora {
    std::map<ClientId, MessageClient*> m_Clients;
    std::shared_mutex m_ClientsMutex;
    std::multimap<MessageSubject, std::pair<ClientId, MessageClient*>> m_Subscriptions;
    std::shared_mutex m_SubscriptionsMutex;

    void MessageHub::Connect(ClientId id, MessageClient *client)
    {
        m_ClientsMutex.lock();
        m_Clients[id] = client;
        m_ClientsMutex.unlock();
    }

    void MessageHub::Disconnect(ClientId id)
    {

    }

    void SendCommand(Message &m)
    {
        m_ClientsMutex.lock_shared();
        auto client = m_Clients.find(m.to());
        if (client != m_Clients.end())
        {
            client->second->Receive(m);
        }
        m_ClientsMutex.unlock_shared();
    }

    void SendEvent(MessageSubject s, Message &m)
    {
        m_SubscriptionsMutex.lock_shared();
        auto subscriptors = m_Subscriptions.equal_range(s);

        std::for_each(subscriptors.first, subscriptors.second, [&m](auto & subscriptor)
        {
            if (subscriptor.second.first != m.from()) subscriptor.second.second->Receive(m);
        });
        m_SubscriptionsMutex.unlock_shared();
    }

    void MessageHub::Send(Message &m)
    {
        switch(m.type()) {
            case MessageType::COMMAND:
                SendCommand(m);
                break;
            case MessageType::EVENT:
                SendEvent("all", m);
                SendEvent(m.subject(), m);
                break;
            default:
                break;
        }
    }

    void MessageHub::Subscribe(ClientId id, MessageSubject &s)
    {
        m_ClientsMutex.lock();
        auto client = m_Clients.find(id);
        if (client != m_Clients.end())
        {
            m_SubscriptionsMutex.lock();
            m_Subscriptions.insert(std::make_pair(s, std::make_pair(id, client->second)));
            m_SubscriptionsMutex.unlock();
        }
        m_ClientsMutex.unlock();
    }

    void MessageHub::Desubscribe(ClientId id, MessageSubject &s)
    {
        m_SubscriptionsMutex.lock();
        auto subscriptors = m_Subscriptions.equal_range(s);
        for (auto it = subscriptors.first; it != subscriptors.second; it++)
        {
            if (it->second.first == id)
            {
                m_Subscriptions.erase(it);
                break;
            }
        }
        m_SubscriptionsMutex.unlock();
    }

}
