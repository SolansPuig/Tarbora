#pragma once
#include "Scene.hpp"

namespace Tarbora {
    class Skybox
    {
    public:
        Skybox(Scene &scene, std::string shader, std::string texture)
        {
            m_Mesh.reset(new MeshNode(SKY_ID, "sky", "meshes/cube.mesh"));
            m_Material.reset(new MaterialNode(SKY_ID, "sky_material", shader, texture));
            m_Material->AddChild(m_Mesh);
            scene.AddChild(m_Material, RenderPass::Sky);
        }

    protected:
        std::shared_ptr<MaterialNode> m_Material;
        std::shared_ptr<MeshNode> m_Mesh;
    };

    typedef std::shared_ptr<Skybox> SkyboxPtr;
}
