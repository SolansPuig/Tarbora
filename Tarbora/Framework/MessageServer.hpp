#pragma once

#include "AbstractModule.hpp"

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "Protos/messages.grpc.pb.h"

using tbMessages::TarboraMessages;
using tbMessages::Message;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;
typedef grpc::ServerReaderWriter<Message, Message> StreamServer;

namespace Tarbora {
    class Server final : public TarboraMessages::Service {
    public:
        Server() {}

        grpc::Status Connect(grpc::ServerContext *context, StreamServer *stream) override;

        grpc::Status Subscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response);

        grpc::Status Desubscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response);

    private:
        void Send(StreamServer *stream, MessageSubject s, Message m);

        std::map<ClientId, StreamServer*> m_Clients;
        std::multimap<MessageSubject, StreamServer*> m_Subscriptions;
        std::mutex m_ClientsMutex;
        std::mutex m_SubscriptionsMutex;
    };

    class MessageServer
    {
    public:
        MessageServer(const std::string serverAddress);
        ~MessageServer();

        void Run();

    private:
        Server m_Service;
        grpc::ServerBuilder m_Builder;
        const std::string m_Address;
        std::unique_ptr<grpc::Server> m_Server;
    };
}
