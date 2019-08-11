#pragma once
#include "Application.hpp"
#include "MessageManager/inc/MessageManager.hpp"
#include "MessageManager/inc/BasicMessages.hpp"
#include "ResourceManager/inc/Resource.hpp"
#include "ResourceManager/inc/Json.hpp"

namespace Tarbora {
    class ClientApplication : public Application
    {
    public:
        ClientApplication(unsigned int client_id, std::string server_address);
        ~ClientApplication();

        virtual void Run() override;

        virtual void GetInput() {}
        virtual void Update(float elapsed_time) = 0;
        virtual void Draw() {}

        std::shared_ptr<MessageManagerImpl> MessageManager() { return m_MessageManager; }

        // ResourceManager resourceManager;

    private:
        std::chrono::high_resolution_clock::time_point m_time;

        std::shared_ptr<MessageManagerImpl> m_MessageManager;
    };
}
