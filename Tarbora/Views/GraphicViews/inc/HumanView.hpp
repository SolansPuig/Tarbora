#pragma once
#include "Layer.hpp"
#include "GraphicView.hpp"
#include "GameLayer.hpp"
#include "Console.hpp"

namespace Tarbora {
    class HumanView : public GraphicView
    {
    public:
        HumanView(std::string server_address);
        ~HumanView();

        virtual void GetInput() override;
        virtual void Update(float elapsed_time) override;
        virtual void Draw() override;

        virtual void PushLayer(std::shared_ptr<Layer> layer);
        virtual void RemoveLayer(std::shared_ptr<Layer> layer);

        std::shared_ptr<GameLayerImpl> GameLayer() { return m_GameLayer; }
        std::shared_ptr<ConsoleImpl> Console() { return m_Console; }

    protected:
        LayerList m_Layers;
        std::shared_ptr<GameLayerImpl> m_GameLayer;
        std::shared_ptr<ConsoleImpl> m_Console;
    };
}
