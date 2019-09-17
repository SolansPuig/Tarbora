#pragma once
#include "AbstractModule.hpp"
#include "MessageManager/inc/MessageManager.hpp"
#include "MessageManager/inc/BasicMessages.hpp"
#include "ResourceManager/inc/Resource.hpp"
#include "ResourceManager/inc/Json.hpp"

namespace Tarbora {
    class Module : public AbstractModule
    {
    public:
        Module(unsigned int clientId, const std::string serverAddress);
        ~Module();

        virtual void Run() override;

        virtual void GetInput() {}
        virtual void Update(float elapsedTime) = 0;
        virtual void Draw() {}

        inline void SendMessage(ClientId to, std::string type, MessageBody body) const;
        inline void BoradcastMessage(std::string type, MessageBody body) const;
        std::shared_ptr<MessageManager> GetMessageManager() const { return m_MessageManager; }

        // ResourceManager resourceManager;

    private:
        std::chrono::high_resolution_clock::time_point m_Time;

        std::shared_ptr<MessageManager> m_MessageManager;
    };
}
