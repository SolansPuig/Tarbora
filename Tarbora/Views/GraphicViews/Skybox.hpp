#pragma once
#include "Scene.hpp"

namespace Tarbora {
    class Skybox : public MaterialNode
    {
    public:
        Skybox(const std::string &material)
            : MaterialNode("sky", "sky", material)
        {
            std::shared_ptr<MeshNode> mesh = std::shared_ptr<MeshNode>(new MeshNode("sky", "sky_root", RenderPass::Sky, "cube.mesh"));
            addChild(mesh);
        }
    };
}
