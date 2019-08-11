#include "../inc/MessageManager.hpp"

namespace Tarbora {
    MessageManagerImpl::MessageManagerImpl(ClientApplication *app, int client_id, std::string server_address) :
        app(app)
    {
        m_network_client = std::unique_ptr<NetworkClient>(new NetworkClient(client_id, server_address));
        m_network_client->Connect();
    }

    MessageManagerImpl::~MessageManagerImpl()
    {
        m_network_client->Disconnect();
    }

    void MessageManagerImpl::ReadMessages()
    {
        Message m;
        while (m_network_client->GetMessage(&m))
        {
            auto all = m_listeners.find("all");
            if (all != m_listeners.end())
            {
                for (auto listener : all->second)
                {
                    listener(m.subject(), m.body());
                }
            }

            auto type = m_listeners.find(m.subject());
            if (type != m_listeners.end())
            {
                for (auto listener : type->second)
                {
                    listener(m.subject(), m.body());
                }
            }
        }
    }

    EventId MessageManagerImpl::Subscribe(std::string type, EventFn func)
    {
        auto type_listeners = m_listeners.find(type);
        if (type_listeners == m_listeners.end())
        {
            m_network_client->Subscribe(type);
        }

        m_listeners[type].push_back(func);
        return m_listeners[type].size() - 1;
    }

    void MessageManagerImpl::Desubscribe(std::string type, EventId id)
    {
        m_listeners[type].erase(m_listeners[type].begin() + id);
        if (m_listeners[type].size() == 0)
        {
            m_network_client->Desubscribe(type);
        }
    }

    void MessageManagerImpl::Trigger(std::string type, std::string event)
    {
        Message message;
        message.set_type(MessageType::EVENT);
        message.set_subject(type);
        message.set_body(event);

        m_network_client->Send(message);
    }

     void MessageManagerImpl::Send(unsigned int to, std::string type, std::string command)
    {
        Message message;
        message.set_type(MessageType::COMMAND);
        message.set_to(to);
        message.set_subject(type);
        message.set_body(command);

        m_network_client->Send(message);
    }
}
