#include "MessageHub.hpp"

namespace Tarbora {
    std::unordered_map<ClientId, MessageClient*> clients_;
    std::shared_mutex clients_mutex_;
    std::unordered_multimap<MessageSubject, std::pair<ClientId, MessageClient*>> subscriptions_;
    std::shared_mutex subscriptions_mutex_;

    void MessageHub::connect(const ClientId &id, MessageClient *client)
    {
        clients_mutex_.lock();
        clients_[id] = client;
        clients_mutex_.unlock();
    }

    void MessageHub::disconnect(const ClientId &id)
    {
        UNUSED(id);
        // TODO
    }

    void sendCommand(const Message::Message &m)
    {
        clients_mutex_.lock_shared();
        auto client = clients_.find(m.to());
        if (client != clients_.end())
        {
            client->second->receive(m);
        }
        clients_mutex_.unlock_shared();
    }

    void sendEvent(const MessageSubject &s, const Message::Message &m)
    {
        subscriptions_mutex_.lock_shared();
        auto subscriptors = subscriptions_.equal_range(s);

        std::for_each(subscriptors.first, subscriptors.second, [&m](auto & subscriptor)
        {
            if (subscriptor.second.first != m.from()) subscriptor.second.second->receive(m);
        });
        subscriptions_mutex_.unlock_shared();
    }

    void MessageHub::send(const Message::Message &m)
    {
        switch(m.type()) {
            case Message::MessageType::COMMAND:
                sendCommand(m);
                break;
            case Message::MessageType::EVENT:
                sendEvent("all", m);
                sendEvent(m.subject(), m);
                break;
            default:
                break;
        }
    }

    void MessageHub::subscribe(const ClientId &id, const MessageSubject &s)
    {
        clients_mutex_.lock();
        auto client = clients_.find(id);
        if (client != clients_.end())
        {
            subscriptions_mutex_.lock();
            subscriptions_.insert(std::make_pair(s, std::make_pair(id, client->second)));
            subscriptions_mutex_.unlock();
        }
        clients_mutex_.unlock();
    }

    void MessageHub::desubscribe(const ClientId &id, const MessageSubject &s)
    {
        subscriptions_mutex_.lock();
        const auto subscriptors = subscriptions_.equal_range(s);
        for (auto it = subscriptors.first; it != subscriptors.second; it++)
        {
            if (it->second.first == id)
            {
                subscriptions_.erase(it);
                break;
            }
        }
        subscriptions_mutex_.unlock();
    }

}
