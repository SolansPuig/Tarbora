#pragma once
#include "Scene.hpp"
#include "Layer.hpp"

namespace Tarbora {
    class GameLayer : public Layer
    {
    public:
        GameLayer(HumanView *view, bool start_active=true);
        ~GameLayer() {}

        virtual bool onMessage(const MessageBody &m) override;

        virtual void getInput() override;
        virtual void update(float delta_time) override;
        virtual void draw() override;

        void setTargetId(ActorId id) { target_id_ = id; }
        const ActorId& getTargetId() const { return target_id_; }

        std::shared_ptr<Scene> getScene() { return scene_; }

        void freezeMouse(bool freeze) {freeze_mouse_ = freeze; }
    private:
        std::shared_ptr<Scene> scene_;

        ActorId target_id_;
        glm::vec2 look_direction_;
        glm::vec3 movement_;
        bool jump_;
        bool freeze_mouse_;
        bool pick_object_{false};
    };
}
