#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/security/credentials.h>

#include "Protos/messages.grpc.pb.h"

#include "CommonHeaders.hpp"
#include "Global.hpp"

using grpc::ClientContext;
using grpc::ClientReaderWriter;
typedef ClientReaderWriter<tbMessages::Message, tbMessages::Message> Stream;

class MessageClient {
public:
    MessageClient(ClientId id, std::string serverAddress);

    void Connect();
    void Disconnect();

    bool GetMessage(tbMessages::Message *message);
    void Send(tbMessages::Message &message);

    void Subscribe(MessageSubject &s);
    void Desubscribe(MessageSubject &s);

private:
    void MessageHandler();

    int m_Id;

    // Connection related
    ClientContext m_Context;
    std::unique_ptr<tbMessages::TarboraMessages::Stub> m_Stub;
    std::unique_ptr<Stream> m_Stream;

    // Thread to read the messages
    std::thread m_Thread;

    // Entry message queue
    std::queue<tbMessages::Message> m_Messages;
    std::mutex m_MessagesMutex;
};
