#include "Module.hpp"

namespace Tarbora {
    Module::Module(ClientId id, const std::string serverAddress) :
        AbstractModule()
    {
        m_MessageManager = std::shared_ptr<MessageManager>(new MessageManager(this, id, serverAddress));
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
}
