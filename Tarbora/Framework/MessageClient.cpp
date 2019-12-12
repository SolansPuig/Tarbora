#include "MessageClient.hpp"
#include "MessageHub.hpp"

using tbMessages::Message;
using tbMessages::TarboraMessages;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;

MessageClient::MessageClient(ClientId id)
    : m_Id(id)
{}

void MessageClient::Connect()
{
    MessageHub::Connect(m_Id, this);
}

void MessageClient::Disconnect()
{
    MessageHub::Disconnect(m_Id);
}

bool MessageClient::GetMessage(Message *message)
{
    ZoneScoped;
    if (m_Messages.empty())
    {
        return false;
    }
    *message = m_Messages.front();
    m_Messages.pop();
    return true;
}

void MessageClient::Send(Message &message)
{
    MessageHub::Send(message);
}

void MessageClient::Receive(Message &message)
{
    m_Messages.push(message);
}

void MessageClient::Subscribe(MessageSubject &s) {
    MessageHub::Subscribe(m_Id, s);
}

void MessageClient::Desubscribe(MessageSubject &s) {
    MessageHub::Desubscribe(m_Id, s);
}
