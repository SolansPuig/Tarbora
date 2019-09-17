#pragma once
#include "Scene.hpp"
#include "Layer.hpp"

namespace Tarbora {
    class GameLayerImpl : public Layer
    {
    public:
        GameLayerImpl(GraphicView *view, bool start_active=true);
        ~GameLayerImpl();

        virtual bool OnMessage(Message *m) override;

        virtual void GetInput() override;
        virtual void Update(float deltaTime) override;
        virtual void Draw() override;

        void SetTargetId(ActorId id) { m_TargetId = id; }
        ActorId GetTargetId() const { return m_TargetId; }
    private:
        std::unique_ptr<Scene> m_Scene;

        ActorId m_TargetId;
        float m_LastX, m_LastY; // Last mouse position
        glm::vec3 m_Rotation;
        glm::vec3 m_Movement;
        bool m_Jump;
        float m_JumpImpulse, m_DirectionImpulse, m_TorqueImpulse;

        EventId m_CreateActor, m_SetCamera, m_MoveActor, m_DeleteActor, m_ActorAnimation;
    };
}
