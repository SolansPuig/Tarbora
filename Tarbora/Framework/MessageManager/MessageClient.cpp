#include "MessageClient.hpp"
#include "MessageHub.hpp"
#include "messages.pb.h"

namespace Tarbora {
    MessageClient::MessageClient(const ClientId &id)
        : id_(id)
    {}

    void MessageClient::connect()
    {
        MessageHub::connect(id_, this);
    }

    void MessageClient::disconnect()
    {
        MessageHub::disconnect(id_);
    }

    bool MessageClient::getMessage(Message::Message *message)
    {
        ZoneScoped;
        if (messages_.empty())
        {
            return false;
        }
        *message = messages_.front();
        messages_.pop();
        return true;
    }

    void MessageClient::send(const Message::Message &message)
    {
        MessageHub::send(message);
    }

    void MessageClient::receive(const Message::Message &message)
    {
        messages_.push(message);
    }

    void MessageClient::subscribe(const MessageSubject &s) {
        MessageHub::subscribe(id_, s);
    }

    void MessageClient::desubscribe(const MessageSubject &s) {
        MessageHub::desubscribe(id_, s);
    }

}
