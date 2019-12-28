#pragma once
#include "../../../Framework/Module.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
    class ActorModel : public MaterialNode
    {
        friend class AnimationController;
    public:
        ActorModel(ActorId id, RenderPass renderPass, const std::string &model, const std::string &material);

        virtual void Update(Scene *scene, float deltaTime) override;

        virtual SceneNodePtr GetChild(const std::string &name) override;

        void Animate(const std::string &animation, const std::string &animations_file="");

    private:
        std::shared_ptr<MeshNode> CreateNode(ActorId id, RenderPass renderPass, LuaTable table, float pixelDensity, float textureSize);
        std::shared_ptr<Camera> CreateCamera(ActorId id, LuaTable table);

        std::map<std::string, std::shared_ptr<SceneNode>> m_Nodes;

        std::optional<AnimationController> m_AnimationController;

        int m_PixelDensity;
    };
}
