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
            renderer_(renderer) {}

        void drawMesh(
            RenderPass render_pass,
            ResourcePtr<Mesh> mesh,
            const glm::mat4 &transform,
            const glm::tvec2<unsigned short> &uv,
            const glm::vec3 &mesh_size,
            const glm::vec3 &texture_size,
            const glm::tvec3<unsigned char> &primary,
            const glm::tvec3<unsigned char> &secondary,
            const glm::tvec3<unsigned char> &detail,
            const glm::tvec3<unsigned char> &detail2
        );

        void draw();

        void pushMaterial(ResourcePtr<Material> material);
        ResourcePtr<Material> getActiveMaterial();
        void popMaterial();

        void setProjectionMatrix(const glm::mat4 &m);
        void setViewMatrix(const glm::mat4 &m);

    private:
        RenderList render_list_;
        std::stack<ResourcePtr<Material>> material_stack_;

        std::shared_ptr<Renderer> renderer_;

        glm::mat4 projection_;
        glm::mat4 view_;
    };
}
