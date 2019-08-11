#pragma once
#include "../../Global.hpp"
#include <functional>

#include "NetworkClient.hpp"

namespace Tarbora {
    class ClientApplication;

    typedef std::function<void(std::string, std::string)> EventFn;
    typedef unsigned int EventId;

    class MessageManagerImpl
    {
    public:
        MessageManagerImpl(ClientApplication *app, int client_id, std::string server_address);
        ~MessageManagerImpl();

        void ReadMessages();

        EventId Subscribe(std::string type, EventFn func);
        void Desubscribe(std::string type, EventId id);

        void Trigger(std::string type, std::string event);
        void Send(unsigned int to, std::string type, std::string command);

    private:
        ClientApplication *app;

        std::unique_ptr<NetworkClient> m_network_client;

        std::map<std::string, std::vector<EventFn>> m_listeners;
    };
}
