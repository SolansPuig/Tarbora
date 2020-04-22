#include "MessageManager.hpp"
#include "messages.pb.h"

namespace Tarbora {
    MessageManager::MessageManager(const ClientId &id)
    {
        message_client_ = std::unique_ptr<MessageClient>(new MessageClient(id));
        message_client_->connect();
    }

    MessageManager::~MessageManager()
    {
        message_client_->disconnect();
    }

    void MessageManager::readMessage(const Message::Message &m)
    {
        const MessageBody b(m.body());

        //  First execute the callback for the listeners subscribed to all msgs
        auto all_listeners = listeners_.find("all");
        if (all_listeners != listeners_.end())
        {
            for (auto listener : all_listeners->second)
            {
                listener(m.subject(), b);
            }
        }

        // Now execute the callback for the listeners subscribed to this msg
        auto subject_listeners = listeners_.find(m.subject());
        if (subject_listeners != listeners_.end())
        {
            for (auto listener : subject_listeners->second)
            {
                listener(m.subject(), b);
            }
        }
    }

    void MessageManager::readMessages()
    {
        Message::Message m;
        while (message_client_->getMessage(&m))
        {
            readMessage(m);
        }
    }

    SubscriptorId MessageManager::subscribe(const MessageSubject &s, MessageFn func, bool local)
    {
        // If this module was still not subscribed to this msg, do it
        const auto subject_listeners = listeners_.find(s);
        if (!local && subject_listeners == listeners_.end())
        {
            message_client_->subscribe(s);
        }

        // Anyways, register a new callback function
        listeners_[s].push_back(func);
        return listeners_[s].size() - 1;
    }

    void MessageManager::desubscribe(const MessageSubject &s, const SubscriptorId &id)
    {
        listeners_[s].erase(listeners_[s].begin() + id);
        if (listeners_[s].size() == 0)
        {
            message_client_->desubscribe(s);
        }
    }

    void MessageManager::trigger(const MessageSubject &s, const MessageBody &b)
    {
        Message::Message message;
        message.set_type(Message::MessageType::EVENT);
        message.set_subject(s);
        message.set_body(b.getContent());

        message_client_->send(message);
    }

    void MessageManager::send(const ClientId &to, const MessageSubject &s, const MessageBody &b)
    {
        Message::Message message;
        message.set_type(Message::MessageType::COMMAND);
        message.set_to(to);
        message.set_subject(s);
        message.set_body(b.getContent());

        message_client_->send(message);
    }

    void MessageManager::triggerLocal(const MessageSubject &s, const MessageBody &b)
    {
        Message::Message message;
        message.set_type(Message::MessageType::LOCAL);
        message.set_subject(s);
        message.set_body(b.getContent());

        // It's local, so there's no need to send it to the server. Also,
        // this way its execution is immediate (important!)
        readMessage(message);
    }
}
