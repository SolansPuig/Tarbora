#pragma once
#include "Scene.hpp"

namespace Tarbora {
    class Skybox : public MaterialNode
    {
    public:
        Skybox(std::string shader, std::string texture)
            : MaterialNode(SKY_ID, "sky", shader, texture)
        {
            std::shared_ptr<MeshNode> mesh = std::shared_ptr<MeshNode>(new MeshNode(SKY_ID, "sky_root", "cube.mesh"));
            AddChild(mesh);
        }
    };
}
