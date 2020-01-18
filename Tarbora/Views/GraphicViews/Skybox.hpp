#pragma once
#include "Scene.hpp"

namespace Tarbora {
    class Skybox : public MaterialNode
    {
    public:
        Skybox(std::string material)
            : MaterialNode(SKY_ID, "sky", material)
        {
            std::shared_ptr<MeshNode> mesh = std::shared_ptr<MeshNode>(new MeshNode(SKY_ID, "sky_root", RenderPass::Sky, "cube.mesh"));
            AddChild(mesh);
        }
    };
}
