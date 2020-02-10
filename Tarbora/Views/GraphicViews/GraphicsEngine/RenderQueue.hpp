#pragma once

// #define NO_INSTANCED true

#include "../../../Framework/Global.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Renderer.hpp"

namespace Tarbora {
    enum RenderPass
    {
        Zero,
        Static = Zero,
        Actor,
        NoCulling,
        Sky,
        Transparent,
        NotRendered,
        Last
    };

    typedef std::vector<RenderElementData> InstanceList;
    typedef std::unordered_map<ResourcePtr<Mesh>, InstanceList, ResourcePtrHash> MeshList;
    typedef std::unordered_map<ResourcePtr<Material>, MeshList, ResourcePtrHash> MaterialList;
    typedef std::map<unsigned int, MaterialList> RenderList;

    class RenderQueue
    {
    public:
        RenderQueue(std::shared_ptr<Renderer> renderer) :
            m_Renderer(renderer) {}

        void DrawMesh(RenderPass renderPass, ResourcePtr<Mesh> mesh, glm::mat4 transform, glm::vec2 uv, glm::vec3 meshSize, glm::vec3 textureSize, glm::vec3 primary, glm::vec3 secondary, glm::vec3 detail, glm::vec3 detail2);

        void Draw();

        void PushMaterial(ResourcePtr<Material> material);
        ResourcePtr<Material> GetActiveMaterial();
        void PopMaterial();

        void SetProjectionMatrix(const glm::mat4 &m);
        void SetViewMatrix(const glm::mat4 &m);

    private:
        RenderList m_RenderList;
        std::stack<ResourcePtr<Material>> m_MaterialStack;

        std::shared_ptr<Renderer> m_Renderer;

        glm::mat4 m_Projection;
        glm::mat4 m_View;
    };
}
