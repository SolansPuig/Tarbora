#pragma once
#include "../../Framework/Module.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
    class ActorModel : public MaterialNode
    {
        friend class AnimationController;
    public:
        ActorModel(ActorId id, RenderPass renderPass, const std::string &model, const std::string &material);

        virtual void Update(Scene *scene, float deltaTime) override;

        virtual std::shared_ptr<SceneNode> GetChild(const std::string &name) override;

        void Animate(const std::string &name, const std::string &file="");

    private:
        std::shared_ptr<MeshNode> CreateNode(ActorId id, RenderPass renderPass, LuaTable table);
        std::shared_ptr<Camera> CreateCamera(ActorId id, LuaTable table);

        std::map<std::string, std::shared_ptr<SceneNode>> m_Nodes;

        std::unique_ptr<AnimationController> m_AnimationController;
    };
}
