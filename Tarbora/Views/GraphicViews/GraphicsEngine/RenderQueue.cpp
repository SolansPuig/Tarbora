#include "RenderQueue.hpp"

namespace Tarbora {
    void RenderQueue::drawMesh(
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
    )
    {
        ResourcePtr<Material> material = material_stack_.top();

        RenderElementData data;
        data.transform = transform;
        data.uv_map = uv;
        data.mesh_size = mesh_size;
        data.texture_size = texture_size;
        data.color_primary = primary;
        data.color_secondary = secondary;
        data.color_detail = detail;
        data.color_detail2 = detail2;

        render_list_[render_pass][material][mesh].emplace_back(data);
    }

    void RenderQueue::draw()
    {
        for (unsigned int pass = RenderPass::Zero; pass < RenderPass::Last; ++pass)
        {
            switch (pass)
            {
                case Static:
                    renderer_->geometryPass();
                    renderer_->setFaceCulling(true);
                    break;
                case Actor:
                    break;
                case NoCulling:
                    renderer_->setFaceCulling(false);
                    break;
                case Sky:
                    renderer_->occlusionPass();
                    renderer_->lightingPass();
                    renderer_->scenePass();
                    renderer_->sky();
                    break;
                case Transparent:
                    renderer_->cleanSky();
                    break;
                default:
                    break;
            }

            for (auto material : render_list_[pass])
            {
                material.first->bind(projection_, view_);
                for (auto mesh : material.second)
                {
                    mesh.first->bind();
                    for (auto instance : mesh.second)
                    {
                        mesh.first->addInstance(instance);
                    }
                    mesh.first->drawInstanced();
                }
            }
        }
        render_list_.clear();
    }

    void RenderQueue::pushMaterial(ResourcePtr<Material> material)
    {
        material_stack_.push(material);
    }

    ResourcePtr<Material> RenderQueue::getActiveMaterial()
    {
        return material_stack_.top();
    }

    void RenderQueue::popMaterial()
    {
        material_stack_.pop();
    }

    void RenderQueue::setProjectionMatrix(const glm::mat4 &m)
    {
        renderer_->setProjectionMatrix(m);
        projection_ = m;
    }

    void RenderQueue::setViewMatrix(const glm::mat4 &m)
    {
        renderer_->setViewMatrix(m);
        view_ = m;
    }
}
