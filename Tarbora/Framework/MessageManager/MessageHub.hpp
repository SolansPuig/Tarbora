#pragma once

#include "messages.pb.h"

#include "../CommonHeaders.hpp"
#include "../Global.hpp"
#include "MessageClient.hpp"

namespace Tarbora {
    namespace MessageHub {
        void connect(const ClientId &id, MessageClient *client);
        void disconnect(const ClientId &id);

        void send(const Message::Message &m);

        void subscribe(const ClientId &id, const MessageSubject &s);
        void desubscribe(const ClientId &id, const MessageSubject &s);
    }
}
