#include "Module.hpp"

namespace Tarbora {
    Module::Module(ClientId id) :
        AbstractModule()
    {
        m_MessageManager = std::shared_ptr<MessageManager>(new MessageManager(id));
    }

    Module::~Module() {}

    void Module::Run(std::string name)
    {
        ZoneScoped;

        m_MessageManager->SetDebugName(name + " Message Client");

        m_Time = std::chrono::system_clock::now();
        while(AbstractModule::m_Run)
        {
            auto currentTime = std::chrono::system_clock::now();
            std::chrono::duration<float> duration = currentTime - m_Time;
            float elapsedTime = duration.count();

            if (elapsedTime >= 1./m_MaxFrameRate)
            {
                m_Time = currentTime;

                if (!m_UsingThread) FrameMark;
                FrameMarkNamed(name.c_str());

                m_MessageManager->ReadMessages();
                GetInput();
                Update(elapsedTime);
                Draw();
            }
        }
    }
}
