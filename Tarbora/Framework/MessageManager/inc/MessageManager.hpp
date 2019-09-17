#pragma once
#include "../../Global.hpp"
#include <functional>

#include "NetworkClient.hpp"

namespace Tarbora {
    class Module;

    typedef std::function<void(std::string, std::string)> EventFn;
    typedef unsigned int EventId;

    typedef std::string MessageBody;
    typedef unsigned int ClientId;

    class MessageManager
    {
    public:
        MessageManager(Module *m, ClientId id, std::string serverAddress);
        ~MessageManager();

        void ReadMessages();

        EventId Subscribe(std::string type, EventFn func);
        void Desubscribe(std::string type, EventId id);

        void Trigger(std::string type, MessageBody body);
        void Send(ClientId to, std::string type, MessageBody body);

    private:
        Module *m_Module;
        std::unique_ptr<NetworkClient> m_NetworkClient;

        std::map<std::string, std::vector<EventFn>> m_Listeners;
    };
}
