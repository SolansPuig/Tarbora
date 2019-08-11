#pragma once
#include "../../../Framework/ClientApplication.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
    class ActorModel : public MaterialNode
    {
        friend class AnimationController;
    public:
        ActorModel(ActorId id, std::string model, std::string shader, std::string texture);

        virtual void Update(Scene *scene, float deltaTime) override;

        void Animate(std::string animation, std::string animations_file="");

    private:
        std::shared_ptr<MeshNode> CreateNode(ActorId id, JsonPtr resource, raw_json j, float pixelDensity, float textureSize);

        std::map<std::string, std::shared_ptr<MeshNode>> m_Nodes;

        std::optional<AnimationController> m_AnimationController;

        int m_PixelDensity;
    };
}
