#include "Module.hpp"
#include <chrono>

namespace Tarbora {
    Module::Module(unsigned int clientId, const std::string serverAddress) :
        AbstractModule()
    {
        m_MessageManager = std::shared_ptr<MessageManager>(new MessageManager(this, clientId, serverAddress));
        ResourceManager::Init(this, "../Resources/");
    }

    Module::~Module()
    {
        ResourceManager::Close();
    }

    void Module::Run()
    {
        m_Time = std::chrono::system_clock::now();
        while(AbstractModule::m_Run)
        {
            auto currentTime = std::chrono::system_clock::now();
            std::chrono::duration<float> duration = currentTime - m_Time;
            float elapsedTime = duration.count();
            m_Time = currentTime;

            m_MessageManager->ReadMessages();
            GetInput();
            Update(elapsedTime);
            Draw();
        }
    }

    inline void Module::SendMessage(ClientId to, std::string type, MessageBody body) const
    {
        m_MessageManager->Send(to, type, body);
    }

    inline void Module::BoradcastMessage(std::string type, MessageBody body) const
    {
        m_MessageManager->Trigger(type, body);
    }
}
