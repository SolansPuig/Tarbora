#pragma once

// #define NO_INSTANCED true

#include "../../Framework/Global.hpp"
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

    struct RenderElement
    {
        ResourcePtr<Material> material;
        ResourcePtr<Mesh> mesh;
        glm::mat4 transform;
        glm::vec2 uvMap;
        glm::vec3 textureSize;
        RenderPass renderPass;
        glm::vec3 colorPrimary;
        glm::vec3 colorSecondary;
        glm::vec3 colorDetail;
        glm::vec3 colorDetail2;
    };

    class RenderQueue
    {
    public:
        RenderQueue(std::shared_ptr<Renderer> renderer) :
            m_Renderer(renderer) {}

        void DrawMesh(RenderPass renderPass, ResourcePtr<Mesh> mesh, glm::mat4 transform, glm::vec2 uv, glm::vec3 textureSize, glm::vec3 primary, glm::vec3 secondary, glm::vec3 detail, glm::vec3 detail2);

        void Draw();

        void PushMaterial(ResourcePtr<Material> material);
        ResourcePtr<Material> GetActiveMaterial();
        void PopMaterial();

    private:
        std::vector<RenderElement> m_RenderElements;
        std::stack<ResourcePtr<Material>> m_MaterialStack;

        std::shared_ptr<Renderer> m_Renderer;
    };
}
