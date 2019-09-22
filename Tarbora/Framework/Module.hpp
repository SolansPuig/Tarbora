#pragma once

#include "AbstractModule.hpp"
#include "MessageManager.hpp"
#include "ResourceManager/inc/Resource.hpp"
#include "ResourceManager/inc/Json.hpp"

namespace Tarbora {
    class Module : public AbstractModule
    {
    public:
        Module(ClientId id, const std::string serverAddress);
        ~Module();

        virtual void Run() override;

        virtual void GetInput() {}
        virtual void Update(float elapsedTime) = 0;
        virtual void Draw() {}

        std::shared_ptr<MessageManager> GetMessageManager() const { return m_MessageManager; }

    private:
        std::chrono::high_resolution_clock::time_point m_Time;
        std::shared_ptr<MessageManager> m_MessageManager;
    };
}
