#pragma once
#include "../../Framework/Module/Module.hpp"
#include "ActorModel.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
    class ActorModel : public MaterialNode
    {
        friend class AnimationController;
        friend class Animation;
    public:
        ActorModel(const ActorId &id, RenderPass render_pass, const std::string &model, const std::string &material);
        ~ActorModel();

        std::shared_ptr<MeshNode> createNode(const ActorId &id, RenderPass render_pass, LuaTable table);

        std::shared_ptr<Camera> createCamera(const ActorId &id, LuaTable table);

        virtual void update(Scene *scene, float delta_time) override;

        virtual std::shared_ptr<SceneNode> getChild(const std::string &name) override;

        void startAnimation(Animation animation, bool background=false);

        void endAnimation(const std::string &name, StopMode mode, float fade_out_timer);

        const std::string& getModel() { return model_; }
        RenderPass getRenderPass() { return render_pass_; }

    private:
        std::map<std::string, std::shared_ptr<SceneNode>> nodes_;
        std::unique_ptr<AnimationController> animation_controller_;
        RenderPass render_pass_;
        std::string model_;
    };
}
