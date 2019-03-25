#pragma once
#include "GameView.hpp"
#include "Layer.hpp"
#include <memory>
#include "GameLayer.hpp"

namespace Tarbora {
    class HumanView : public GameView
    {
    public:
        HumanView(ActorId id);
        ~HumanView();

        virtual void Update(float elapsed_time) override;
        virtual void Draw() override;

        virtual ActorId GetTargetId() const override { return m_GameLayer->GetTargetId(); }
        virtual GameViewType GetType() const override { return GV_Human; }

        virtual void PushLayer(std::shared_ptr<Layer> layer);
        virtual void RemoveLayer(std::shared_ptr<Layer> layer);

    protected:
        std::shared_ptr<GameLayer> m_GameLayer;
        LayerList m_Layers;

        // Ids of the event subscription, to allow a desubscription
        unsigned int EvtKeyPressId, EvtKeyReleaseId, EvtButtonPressId, EvtButtonReleaseId, EvtMouseMoveId, EvtMouseScrollId;
    };

    typedef std::shared_ptr<HumanView> HumanViewPtr;
}
