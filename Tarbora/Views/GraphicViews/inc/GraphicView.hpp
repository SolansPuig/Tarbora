#pragma once
#include "../../../Framework/Module.hpp"
#include "../../GraphicsEngine/GraphicsEngine.hpp"

namespace Tarbora {
    class GraphicView : public Module
    {
    public:
        GraphicView(unsigned int client_id, std::string settings_file)
            : Module(client_id)
        {
            m_GraphicsEngine = std::make_shared<GraphicsEngine>(this, settings_file);
        }

        ~GraphicView()
        {
            ResourceManager::Flush();
        }

        std::shared_ptr<GraphicsEngine> GetGraphicsEngine() { return m_GraphicsEngine; }

    private:
        std::shared_ptr<GraphicsEngine> m_GraphicsEngine;
    };
}
