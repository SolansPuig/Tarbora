#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/security/credentials.h>

#include "MessageClient.hpp"

using tbMessages::Message;
using tbMessages::TarboraMessages;
using tbMessages::MessageType;
using tbMessages::EventHeader;
using tbMessages::Empty;

MessageClient::MessageClient(ClientId id, std::string serverAddress)
    : m_Id(id),
      m_Stub(TarboraMessages::NewStub(grpc::CreateChannel(serverAddress, grpc::InsecureChannelCredentials())))
{}

void MessageClient::Connect()
{
    m_Stream = m_Stub->Connect(&m_Context);

    Message message;
    message.set_type(MessageType::START_CONNECTION);
    message.set_from(m_Id);
    m_Stream->Write(message);

    m_Thread = std::thread(&MessageClient::MessageHandler, this);
}

void MessageClient::Disconnect()
{
    Message message;
    message.set_type(MessageType::FINISH_CONNECTION);
    message.set_from(m_Id);
    m_Stream->Write(message);
    m_Stream->Finish();
    m_Thread.join();
}

bool MessageClient::GetMessage(Message *message)
{
    m_MessagesMutex.lock();
    if (m_Messages.empty())
    {
        m_MessagesMutex.unlock();
        return false;
    }
    *message = m_Messages.front();
    m_Messages.pop();
    m_MessagesMutex.unlock();

    return true;
}

void MessageClient::Send(Message &message)
{
    m_Stream->Write(message);
}

void MessageClient::Subscribe(std::string &type) {
    ClientContext context;
    EventHeader message;
    message.set_from(m_Id);
    message.set_subject(type);
    Empty ignore;
    m_Stub->Subscribe(&context, message, &ignore);
}

void MessageClient::Desubscribe(std::string &type) {
    ClientContext context;
    EventHeader message;
    message.set_from(m_Id);
    message.set_subject(type);
    Empty ignore;
    m_Stub->Desubscribe(&context, message, &ignore);
}

void MessageClient::MessageHandler()
{
    Message message;
    while(m_Stream->Read(&message))
    {
        m_MessagesMutex.lock();
        m_Messages.push(message);
        m_MessagesMutex.unlock();
    }
}
