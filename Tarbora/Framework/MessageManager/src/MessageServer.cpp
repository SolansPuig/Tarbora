#include "../inc/MessageServer.hpp"

namespace Tarbora {
    ServerImpl::ServerImpl()
    {

    }

    Status ServerImpl::Connect(ServerContext *context, StreamServer *stream)
    {
        // Read the start connection message. If there's none, ignore it
        Message message;
        stream->Read(&message);
        if (message.type() == MessageType::START_CONNECTION)
        {
            int client_id = message.from();
            LOG_DEBUG("Connection from %d", client_id);

            // Add the client to the client list
            m_clients_mutex.lock();
            m_clients[message.from()] = stream;
            m_clients_mutex.unlock();

            while (stream->Read(&message) && !context->IsCancelled())
            {
                if (message.type() == MessageType::COMMAND)
                {
                    // Pass the message to its receiver
                    m_clients_mutex.lock();
                    auto client = m_clients.find(message.to());
                    if (client != m_clients.end())
                    {
                        client->second->Write(message);
                    }
                    m_clients_mutex.unlock();
                }
                else if (message.type() == MessageType::EVENT)
                {
                    // Pass the message to all the subscriptors
                    m_subscriptions_mutex.lock();
                    auto subscriptors = m_subscriptions.find("all");
                    if (subscriptors != m_subscriptions.end())
                    {
                        for (StreamServer *s : (*subscriptors->second))
                        {
                            if (s != stream)
                            {
                                s->Write(message);
                            }
                        }
                    }

                    subscriptors = m_subscriptions.find(message.subject());
                    if (subscriptors != m_subscriptions.end())
                    {
                        for (StreamServer *s : (*subscriptors->second))
                        {
                            if (s != stream)
                            {
                                s->Write(message);
                            }
                        }
                    }
                    m_subscriptions_mutex.unlock();
                }
                else
                {
                    // End the connection
                    m_clients_mutex.lock();
                    auto client = m_clients.find(message.from());
                    if (client != m_clients.end())
                    {
                        m_clients.erase(client);
                    }
                    m_clients_mutex.unlock();
                    break;
                }
            }
            LOG_DEBUG("Client %d disconnected", client_id);
        }

        return Status::OK;
    }

    Status ServerImpl::Subscribe(ServerContext *context, const EventHeader *event, Empty *response)
    {
        // Find the event to subscribe to
        m_subscriptions_mutex.lock();
        auto subscriptors = m_subscriptions.find(event->subject());
        if (subscriptors == m_subscriptions.end())
        {
            subscriptors = m_subscriptions.emplace(event->subject(), std::unique_ptr<std::vector<StreamServer*>>(new std::vector<StreamServer*>())).first;
        }
        // Find the client that wants to subscribe to the event
        m_clients_mutex.lock();
        auto client = m_clients.find(event->from());
        if (client != m_clients.end())
        {
            // Check that the client isn't already subscribed
            if (std::find(subscriptors->second->begin(), subscriptors->second->end(), client->second) == subscriptors->second->end())
            {
                // Add the client to the subscriptor list
                subscriptors->second->push_back(client->second);
            }
        }
        m_clients_mutex.unlock();

        m_subscriptions_mutex.unlock();

        return Status::OK;
    }

    Status ServerImpl::Desubscribe(ServerContext *context, const EventHeader *event, Empty *response)
    {
        // Find the event to desubscribe to
        m_subscriptions_mutex.lock();
        auto subscriptors = m_subscriptions.find(event->subject());
        if (subscriptors != m_subscriptions.end())
        {
            // Find the client that wants to desubscribe of the event
            m_clients_mutex.lock();
            auto client = m_clients.find(event->from());
            if (client != m_clients.end())
            {
                // Find it on the list and check that the client is already subscribed
                auto goner = std::find(subscriptors->second->begin(), subscriptors->second->end(), client->second);
                if (goner != subscriptors->second->end())
                {
                    // Remove the client from the subscriptor list
                    subscriptors->second->erase(goner);
                }
            }
            m_clients_mutex.unlock();
        }
        m_subscriptions_mutex.unlock();

        return Status::OK;
    }

    MessageServer::MessageServer(const std::string serverAddress)
        : m_Address(serverAddress)
    {
        m_Builder.AddListeningPort(m_Address, grpc::InsecureServerCredentials());
        m_Builder.RegisterService(&m_Service);
        m_Server = std::unique_ptr<Server>(m_Builder.BuildAndStart());
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
