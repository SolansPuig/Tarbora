#pragma once
#include "Scene.hpp"
#include "Layer.hpp"

namespace Tarbora {
    class GameLayerImpl : public Layer
    {
    public:
        GameLayerImpl(GraphicView *view, bool start_active=true);
        ~GameLayerImpl() {}

        virtual bool OnMessage(MessageBody *m) override;

        virtual void GetInput() override;
        virtual void Update(float deltaTime) override;
        virtual void Draw() override;

        void SetTargetId(ActorId id) { m_TargetId = id; }
        ActorId GetTargetId() const { return m_TargetId; }

        void FreezeMouse(bool freeze) {m_FreezeMouse = freeze; }
    private:
        std::unique_ptr<Scene> m_Scene;

        ActorId m_TargetId;
        glm::vec2 m_LookDirection;
        glm::vec3 m_Movement;
        bool m_Jump;
        bool m_FreezeMouse;
    };
}
