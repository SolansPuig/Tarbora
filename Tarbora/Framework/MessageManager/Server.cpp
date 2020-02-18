#include "Server.hpp"

// TODO: Change naming convention to Google's

/*
namespace Tarbora {
    void MessageServer::SendCommand(Message m)
    {
        m_ClientsMutex.lock_shared();
        auto client = m_Clients.find(message.to());
        if (client != m_Clients.end())
        {
            client->second->Write(message);
        }
        m_ClientsMutex.unlock_shared();
    }

    void MessageServer::SendEvent(MessageSubject s, Message m)
    {
        m_SubscriptionsMutex.lock_shared();
        auto subscriptors = m_Subscriptions.equal_range(s);

        std::for_each(subscriptors.first, subscriptors.second, [&m](auto & subscriptor)
        {
            if (subscriptor.second.first != message.from()) subscriptor.second.second->Write(m);
        });
        m_SubscriptionsMutex.unlock_shared();
    }

    grpc::Status MessageServer::Connect(grpc::ServerContext *context, StreamServer *stream)
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
                    SendCommand(message);

                }
                else if (message.type() == MessageType::EVENT)
                {
                    // Pass the message to all the subscriptors
                    SendEvent("all", message);
                    SendEvent(message.subject(), message);
                }
                else
                {
                    // End the connection
                    m_ClientsMutex.lock();
                    auto client = m_Clients.find(message.from());
                    if (client != m_Clients.end())
                    {
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

    grpc::Status MessageServer::Subscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response)
    {
        m_ClientsMutex.lock();
        auto client = m_Clients.find(event->from());
        if (client != m_Clients.end())
        {
            m_SubscriptionsMutex.lock();
            m_Subscriptions.insert(std::make_pair(event->subject(), std::make_pair(event->from(), client->second)));
            m_SubscriptionsMutex.unlock();
        }
        m_ClientsMutex.unlock();

        return grpc::Status::OK;
    }

    grpc::Status MessageServer::Desubscribe(grpc::ServerContext *context, const EventHeader *event, Empty *response)
    {
        m_SubscriptionsMutex.lock();
        auto subscriptors = m_Subscriptions.equal_range(event->subject());
        for (auto it = subscriptors.first; it != subscriptors.second; it++)
        {
            if (it->second.first == event->from())
            {
                m_Subscriptions.erase(it);
                break;
            }
        }
        m_SubscriptionsMutex.unlock();

        return grpc::Status::OK;
    }

    Server::Server(const std::string serverAddress)
        : m_Address(serverAddress)
    {
        m_Builder.AddListeningPort(m_Address, grpc::InsecureServerCredentials());
        m_Builder.RegisterService(&m_Service);
        m_Server = std::unique_ptr<grpc::Server>(m_Builder.BuildAndStart());
    }

    Server::~Server()
    {
        m_Server->Shutdown();
    }

    void Server::Run(std::string name="Unamed Module")
    {
        ZoneScoped;

        LOG_DEBUG("Server listening at %s", m_Address.c_str());
        m_Server->Wait();
    }
}
*/
