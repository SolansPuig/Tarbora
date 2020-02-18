#pragma once
#include "../../Framework/Module/Module.hpp"
#include "GraphicsEngine/GraphicsEngine.hpp"

namespace Tarbora {
    class GraphicView : public Module
    {
    public:
        GraphicView(const ClientId &client_id, const std::string &settings_file)
            : Module(client_id)
        {
            graphics_engine_ = std::make_shared<GraphicsEngine>(this, settings_file);
        }

        std::shared_ptr<GraphicsEngine> getGraphicsEngine() { return graphics_engine_; }

    private:
        std::shared_ptr<GraphicsEngine> graphics_engine_;
    };
}
