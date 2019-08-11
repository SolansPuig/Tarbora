#pragma once

#include <thread>
#include <mutex>
#include <queue>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/security/credentials.h>

#include "../proto/messages.grpc.pb.h"

#include "../../Global.hpp"

using grpc::ClientContext;
using grpc::ClientReaderWriter;
using tbMessages::TarboraMessages;
using tbMessages::Message;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;
typedef ClientReaderWriter<Message, Message> Stream;

class NetworkClient {
public:
    NetworkClient(int client_id, std::string server_address);

    void Connect();
    void Disconnect();

    bool GetMessage(Message *message);
    void Send(Message &message);

    void Subscribe(std::string &type);
    void Desubscribe(std::string &type);

private:
    void MessageHandler();

    int m_Id;

    // Connection related
    ClientContext m_context;
    std::unique_ptr<TarboraMessages::Stub> m_stub;
    std::unique_ptr<Stream> m_stream;

    // Thread to read the messages
    std::thread m_thread;

    // Entry message queue
    std::queue<Message> m_messages;
    std::mutex m_messages_mutex;
};
