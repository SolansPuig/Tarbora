#include <thread>
#include <mutex>

#include "../../Global.hpp"
#include "../../AbstractModule.hpp"
#include "../../Logger.hpp"

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "../proto/messages.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using tbMessages::TarboraMessages;
using tbMessages::Message;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;
typedef ServerReaderWriter<Message, Message> StreamServer;

namespace Tarbora {
    class ServerImpl final : public TarboraMessages::Service {
    public:
        ServerImpl();

        Status Connect(ServerContext *context, StreamServer *stream) override;

        Status Subscribe(ServerContext *context, const EventHeader *event, Empty *response);

        Status Desubscribe(ServerContext *context, const EventHeader *event, Empty *response);

    private:
        void WaitForMessages(StreamServer *stream);

        std::map<unsigned int, StreamServer*> m_clients;
        std::map<std::string, std::unique_ptr<std::vector<StreamServer*>>> m_subscriptions;
        std::mutex m_clients_mutex;
        std::mutex m_subscriptions_mutex;
    };

    class MessageServer
    {
    public:
        MessageServer(const std::string serverAddress);
        ~MessageServer();

        void Run();

    private:
        ServerImpl m_Service;
        ServerBuilder m_Builder;
        const std::string m_Address;
        std::unique_ptr<Server> m_Server;
    };
}
