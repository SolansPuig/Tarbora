#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/security/credentials.h>

#include "../inc/NetworkClient.hpp"

NetworkClient::NetworkClient(int client_id, std::string server_address)
    : m_Id(client_id),
      m_stub(TarboraMessages::NewStub(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())))
{}

void NetworkClient::Connect()
{
    m_stream = m_stub->Connect(&m_context);

    Message message;
    message.set_type(MessageType::START_CONNECTION);
    message.set_from(m_Id);
    m_stream->Write(message);

    m_thread = std::thread(&NetworkClient::MessageHandler, this);
}

void NetworkClient::Disconnect()
{
    Message message;
    message.set_type(MessageType::FINISH_CONNECTION);
    message.set_from(m_Id);
    m_stream->Write(message);
    m_stream->Finish();
    m_thread.join();
}

bool NetworkClient::GetMessage(Message *message)
{
    m_messages_mutex.lock();
    if (m_messages.empty())
    {
        m_messages_mutex.unlock();
        return false;
    }
    *message = m_messages.front();
    m_messages.pop();
    m_messages_mutex.unlock();

    return true;
}

void NetworkClient::Send(Message &message)
{
    m_stream->Write(message);
}

void NetworkClient::Subscribe(std::string &type) {
    ClientContext context;
    EventHeader message;
    message.set_from(m_Id);
    message.set_subject(type);
    Empty ignore;
    m_stub->Subscribe(&context, message, &ignore);
}

void NetworkClient::Desubscribe(std::string &type) {
    ClientContext context;
    EventHeader message;
    message.set_from(m_Id);
    message.set_subject(type);
    Empty ignore;
    m_stub->Desubscribe(&context, message, &ignore);
}

void NetworkClient::MessageHandler()
{
    Message message;
    while(m_stream->Read(&message))
    {
        m_messages_mutex.lock();
        m_messages.push(message);
        m_messages_mutex.unlock();
    }
}
