#pragma once

#include "Game_View.hpp"
#include "Layer.hpp"

namespace Tarbora {
    class Human_View : public Game_View
    {
    public:
        Human_View(float r, float g, float b);
        ~Human_View();

        virtual void OnCreate(GameViewId id) override { m_Id = id; }
        virtual void Update(float elapsed_time) override;
        virtual void Draw() override;

        virtual GameViewId GetId() const override { return m_Id; }
        virtual GameViewType GetType() const override { return GV_Human; }

        virtual void PushLayer(std::shared_ptr<Layer> layer);
        virtual void RemoveLayer(std::shared_ptr<Layer> layer);

    protected:
        GameViewId m_Id;
        LayerList m_Layers;
    };
}
