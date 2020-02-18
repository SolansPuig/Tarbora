#pragma once

#include "MessageClient.hpp"

namespace Tarbora {
    class MessageBody
    {
    public:
        MessageBody(const std::string &body)
            : body_(body) {}

        MessageBody() {}

        const std::string& getContent() const
        {
            return body_;
        }

    protected:
        std::string body_;
    };

    typedef std::function<void(const MessageSubject&, const MessageBody&)> MessageFn;

    class MessageManager
    {
    public:
        MessageManager(const ClientId &id);
        ~MessageManager();

        inline void setDebugName(std::string name="Unamed Message Client")
        {
            message_client_->setDebugName(name);
        }

        void readMessages();

        SubscriptorId subscribe(const MessageSubject &s, MessageFn func, bool local=false);
        void desubscribe(const MessageSubject &s, const SubscriptorId &id);

        void trigger(const MessageSubject &s, const MessageBody &b);
        void send(const ClientId &to, const MessageSubject &s, const MessageBody &b);
        void triggerLocal(const MessageSubject &s, const MessageBody &b);

    private:
        void readMessage(const Message::Message &m);

        std::unique_ptr<MessageClient> message_client_;

        std::unordered_map<std::string, std::vector<MessageFn>> listeners_;
    };
}
