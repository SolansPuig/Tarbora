#pragma once
#include "../../../Framework/ClientApplication.hpp"
#include "../../GraphicsEngine/inc/GraphicsEngine.hpp"

namespace Tarbora {
    class GraphicView : public ClientApplication
    {
    public:
        GraphicView(unsigned int client_id, std::string server_address, std::string settings_file)
            : ClientApplication(client_id, server_address)
        {
            m_GraphicsEngine = std::make_shared<GraphicsEngineImpl>(this, settings_file);
            m_Input = std::make_shared<InputImpl>(this);
            m_GraphicsEngine->InitGui();
        }

        ~GraphicView()
        {
            ResourceManager::Flush();
        }

        std::shared_ptr<GraphicsEngineImpl> GraphicsEngine() { return m_GraphicsEngine; }
        std::shared_ptr<InputImpl> Input() { return m_Input; }

    private:
        std::shared_ptr<GraphicsEngineImpl> m_GraphicsEngine;
        std::shared_ptr<InputImpl> m_Input;
    };
}
