#pragma once

#include "Protos/messages.grpc.pb.h"

#include "CommonHeaders.hpp"
#include "Global.hpp"

class MessageClient {
public:
    MessageClient(ClientId id);
    inline void SetDebugName(std::string name)
    {
        
    }

    void Connect();
    void Disconnect();

    bool GetMessage(tbMessages::Message *message);
    void Send(tbMessages::Message &message);
    void Receive(tbMessages::Message &message);

    void Subscribe(MessageSubject &s);
    void Desubscribe(MessageSubject &s);

private:
    int m_Id;

    std::queue<tbMessages::Message> m_Messages;
};
