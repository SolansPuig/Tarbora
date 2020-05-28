#pragma once
#include "Layer.hpp"
#include "GraphicView.hpp"
#include "GameLayer.hpp"

namespace Tarbora {
    class HumanView : public GraphicView
    {
    public:
        HumanView();
        ~HumanView();

        virtual void getInput() override;
        virtual void update(float delta_time) override;
        virtual void draw() override;

        virtual void pushLayer(std::shared_ptr<Layer> layer);
        virtual void removeLayer(std::shared_ptr<Layer> layer);

        std::shared_ptr<GameLayer> getGameLayer() { return game_layer_; }

    protected:
        std::shared_ptr<GameLayer> game_layer_;
        std::list<std::shared_ptr<Layer>> layers_;
    };
}
