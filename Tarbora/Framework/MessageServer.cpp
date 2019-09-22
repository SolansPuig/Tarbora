#include "MessageServer.hpp"

namespace Tarbora {
    grpc::Status Server::Connect(grpc::ServerContext *context, StreamServer *stream)
    {
        // Read the start connection message. If there's none, ignore it
        Message message;
        stream->Read(&message);
        if (message.type() == MessageType::START_CONNECTION)
        {
            ClientId id = message.from();
            LOG_DEBUG("Connection from %d", id);

            // Add the client to the client list
            m_ClientsMutex.lock();
            m_Clients[message.from()] = stream;
            m_ClientsMutex.unlock();

            while (stream->Read(&message) && !context->IsCancelled())
            {
                if (message.type() == MessageType::COMMAND)
                {
                    // Pass the message to its receiver
                    m_ClientsMutex.lock();
                    auto client = m_Clients.find(message.to());
                    if (client != m_Clients.end())
                    {
                        client->second->Write(message);
                    }
                    m_ClientsMutex.unlock();
                }
                else if (message.type() == MessageType::EVENT)
                {
                    // Pass the message to all the subscriptors
                    Send(stream, "all", message);
                    Send(stream, message.subject(), message);
                }
                else
                {
                    // End the connection
                    m_ClientsMutex.lock();
                    auto client = m_Clients.find(message.from());
                    if (client != m_Clients.end())
                    {
                        // Probably todo: Desubscribe from everywhere

                        m_Clients.erase(client);
                    }
                    m_ClientsMutex.unlock();
                    break;
                }
            }
            LOG_DEBUG("Client %d disconnected", id);
        }

        return grpc::Status::OK;
    }

    grpc::Status Server::Subscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response)
    {
        m_ClientsMutex.lock();
        auto client = m_Clients.find(event->from());
        if (client != m_Clients.end())
        {
            m_SubscriptionsMutex.lock();
            m_Subscriptions.insert(std::make_pair(event->subject(), client->second));
            m_SubscriptionsMutex.unlock();
        }
        m_ClientsMutex.unlock();

        return grpc::Status::OK;
    }

    grpc::Status Server::Desubscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response)
    {
        m_ClientsMutex.lock();
        auto client = m_Clients.find(event->from());
        if (client != m_Clients.end())
        {
            m_SubscriptionsMutex.lock();
            auto subscriptors = m_Subscriptions.equal_range(event->subject());
            for (auto it = subscriptors.first; it != subscriptors.second; it++)
            {
                if (it->second == client->second)
                {
                    m_Subscriptions.erase(it);
                    break;
                }
            }
            m_SubscriptionsMutex.unlock();
        }
        m_ClientsMutex.unlock();

        return grpc::Status::OK;
    }

    void Server::Send(StreamServer *stream, MessageSubject s, Message m)
    {
        m_SubscriptionsMutex.lock();
        auto subscriptors = m_Subscriptions.equal_range(s);
        std::for_each(subscriptors.first, subscriptors.second, [&stream, &m](auto & subscriptorEntry)
        {
            auto & subscriptor = subscriptorEntry.second;
            if (subscriptor != stream) subscriptor->Write(m);
        });
        m_SubscriptionsMutex.unlock();
    }

    MessageServer::MessageServer(const std::string serverAddress)
        : m_Address(serverAddress)
    {
        m_Builder.AddListeningPort(m_Address, grpc::InsecureServerCredentials());
        m_Builder.RegisterService(&m_Service);
        m_Server = std::unique_ptr<grpc::Server>(m_Builder.BuildAndStart());
    }

    MessageServer::~MessageServer()
    {
        m_Server->Shutdown();
    }

    void MessageServer::Run()
    {
        LOG_DEBUG("Server listening at %s", m_Address.c_str());
        m_Server->Wait();
    }
}
