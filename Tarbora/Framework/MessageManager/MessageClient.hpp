#pragma once

#include "../CommonHeaders.hpp"
#include "../Global.hpp"

namespace Message { class Message; }

namespace Tarbora {
    class MessageClient {
    public:
        MessageClient(const ClientId &id);

        void connect();
        void disconnect();

        bool getMessage(Message::Message *message);
        void send(const Message::Message &message);
        void receive(const Message::Message &message);

        void subscribe(const MessageSubject &s);
        void desubscribe(const MessageSubject &s);

        inline void setDebugName(std::string /*name="Unamed Message Client"*/)
        {
            // TODO
        }

    private:
        std::queue<Message::Message> messages_;

        ClientId id_;
    };

}
