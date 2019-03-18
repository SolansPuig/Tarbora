#pragma once
#include "Game_View.hpp"
#include "Layer.hpp"
#include <memory>

namespace Tarbora {
    class Human_View : public Game_View
    {
    public:
        Human_View();
        ~Human_View();

        virtual void OnCreate(GameViewId id) override;
        virtual void OnDestroy() override;
        virtual void Update(float elapsed_time) override;
        virtual void Draw() override;

        virtual GameViewId GetId() const override { return m_Id; }
        virtual GameViewType GetType() const override { return GV_Human; }

        virtual void PushLayer(std::shared_ptr<Layer> layer);
        virtual void RemoveLayer(std::shared_ptr<Layer> layer);

    protected:
        GameViewId m_Id;
        LayerList m_Layers;

        // Ids of the event subscription, to allow a desubscription
        unsigned int EvtKeyPressId, EvtKeyReleaseId, EvtButtonPressId, EvtButtonReleaseId, EvtMouseMoveId, EvtMouseScrollId;
    };

    typedef std::shared_ptr<Human_View> HumanViewPtr;
}
