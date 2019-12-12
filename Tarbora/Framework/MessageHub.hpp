#pragma once

#include "Protos/messages.grpc.pb.h"

#include "CommonHeaders.hpp"
#include "Global.hpp"
#include "MessageClient.hpp"

namespace MessageHub {
    void Connect(ClientId id, MessageClient *client);
    void Disconnect(ClientId id);

    void Send(tbMessages::Message &message);

    void Subscribe(ClientId id, MessageSubject &s);
    void Desubscribe(ClientId id, MessageSubject &s);
}
