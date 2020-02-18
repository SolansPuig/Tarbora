#pragma once
#include "../../Framework/Module/Module.hpp"
#include "ActorModel.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
    class ActorModel : public MaterialNode
    {
        friend class AnimationController;
    public:
        ActorModel(const ActorId &id, RenderPass render_pass, const std::string &model, const std::string &material);

        std::shared_ptr<MeshNode> createNode(const ActorId &id, RenderPass render_pass, LuaTable table);

        std::shared_ptr<Camera> createCamera(const ActorId &id, LuaTable table);

        virtual void update(Scene *scene, float delta_time) override;

        virtual std::shared_ptr<SceneNode> getChild(const std::string &name) override;

        void animate(const std::string &name, const std::string &file="");

    private:
        std::map<std::string, std::shared_ptr<SceneNode>> nodes_;
        std::unique_ptr<AnimationController> animation_controller_;
    };
}
