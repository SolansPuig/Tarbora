#pragma once

// TODO: Change naming convention to Google's

/*
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
    class MessageServer final : public TarboraMessages::Service {
    public:
        MessageServer() {}

        grpc::Status Connect(grpc::ServerContext *context, StreamServer *stream) override;

        grpc::Status Subscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response);

        grpc::Status Desubscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response);

    private:
        void SendCommand(Message m);
        void SendEvent(MessageSubject s, Message m);

        std::map<ClientId, StreamServer*> m_Clients;
        std::multimap<MessageSubject, std::pair<ClientId, StreamServer*>> m_Subscriptions;
        std::shared_mutex m_ClientsMutex;
        std::shared_mutex m_SubscriptionsMutex;
    };

    class Server : public AbstractModule
    {
    public:
        Server(const std::string serverAddress);
        ~Server();

        void Run(std::string name="Unamed Module");

    private:
        MessageServer m_Service;
        grpc::ServerBuilder m_Builder;
        const std::string m_Address;
        std::unique_ptr<grpc::Server> m_Server;
    };
}

*/
