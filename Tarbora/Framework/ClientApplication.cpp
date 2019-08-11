#include "ClientApplication.hpp"
#include <chrono>

namespace Tarbora {
    ClientApplication::ClientApplication(unsigned int client_id, std::string server_address) :
        Application()
    {
        m_MessageManager = std::shared_ptr<MessageManagerImpl>(new MessageManagerImpl(this, client_id, server_address));
        ResourceManager::Init(this, "../Resources/");
    }

    ClientApplication::~ClientApplication()
    {
        ResourceManager::Close();
    }

    void ClientApplication::Run()
    {
        m_time = std::chrono::system_clock::now();
        while(Application::m_run)
        {
            auto current_time = std::chrono::system_clock::now();
            std::chrono::duration<float> duration = current_time - m_time;
            float elapsed_time = duration.count();
            m_time = current_time;

            MessageManager()->ReadMessages();
            GetInput();
            Update(elapsed_time);
            Draw();
        }
    }
}
