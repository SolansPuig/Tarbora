#include "RenderQueue.hpp"

namespace Tarbora {
    void RenderQueue::DrawMesh(RenderPass renderPass, ResourcePtr<Mesh> mesh, glm::mat4 transform, glm::vec2 uv, glm::vec3 textureSize, glm::vec3 primary, glm::vec3 secondary, glm::vec3 detail, glm::vec3 detail2)
    {
        ResourcePtr<Material> material = m_MaterialStack.top();

        #ifndef NO_INSTANCED
            RenderElement element;
            element.material = material;
            element.mesh = mesh;
            element.transform = transform;
            element.uvMap = uv;
            element.textureSize = textureSize;
            element.renderPass = renderPass;
            element.colorPrimary = primary;
            element.colorSecondary = secondary;
            element.colorDetail = detail;
            element.colorDetail2 = detail2;
            m_RenderElements.push_back(element);
        #else
            material->GetShader()->Set("transform", transform);
            material->GetShader()->Set("uv", uv);
            material->GetShader()->Set("size", textureSize);
            material->GetShader()->Set("colorPrimary", primary);
            material->GetShader()->Set("colorSecondary", secondary);
            material->GetShader()->Set("colorDetail", detail);
            material->GetShader()->Set("colorDetail2", detail2);
            mesh->Draw();
        #endif
    }

    bool sortElements(RenderElement e1, RenderElement e2)
    {
        if (e1.renderPass != e2.renderPass)
            return e1.renderPass < e2.renderPass;
        if (e1.material != e2.material)
            return e1.material > e2.material;
        return e1.mesh > e2.mesh;
    }

    void RenderQueue::Draw()
    {
        #ifndef NO_INSTANCED
            std::sort(m_RenderElements.begin(), m_RenderElements.end(), sortElements);
            auto element = m_RenderElements.begin();
            for (unsigned int pass = RenderPass::Zero; pass < RenderPass::Last; pass++)
            {
                ResourcePtr<Material> currentMaterial;
                ResourcePtr<Mesh> currentMesh;

                switch (pass)
                {
                    case Static:
                        m_Renderer->GeometryPass();
                        m_Renderer->SetFaceCulling(true);
                        break;
                    case Actor:
                        break;
                    case NoCulling:
                        m_Renderer->SetFaceCulling(false);
                        break;
                    case Sky:
                        m_Renderer->OcclusionPass();
                        m_Renderer->LightingPass();
                        m_Renderer->ScenePass();
                        m_Renderer->Sky();
                        break;
                    case Transparent:
                        m_Renderer->CleanSky();
                        break;
                    default:
                        break;
                }

                while (element->renderPass == pass)
                {
                    if (element->mesh != currentMesh)
                    {
                        currentMesh = element->mesh;
                        currentMesh->Bind();
                    }
                    if (element->material != currentMaterial)
                    {
                        currentMaterial = element->material;
                        currentMaterial->Bind();
                    }

                    currentMaterial->GetShader()->Set("transform", element->transform);
                    currentMaterial->GetShader()->Set("uv", element->uvMap);
                    currentMaterial->GetShader()->Set("size", element->textureSize);
                    currentMaterial->GetShader()->Set("colorPrimary", element->colorPrimary);
                    currentMaterial->GetShader()->Set("colorSecondary", element->colorSecondary);
                    currentMaterial->GetShader()->Set("colorDetail", element->colorDetail);
                    currentMaterial->GetShader()->Set("colorDetail2", element->colorDetail2);
                    currentMesh->Draw();

                    element++;
                }
            }

            m_RenderElements.clear();
        #endif
    }

    void RenderQueue::PushMaterial(ResourcePtr<Material> material)
    {
        m_MaterialStack.push(material);

        #ifdef NO_INSTANCED
            material->Bind();
        #endif
    }

    ResourcePtr<Material> RenderQueue::GetActiveMaterial()
    {
        return m_MaterialStack.top();
    }

    void RenderQueue::PopMaterial()
    {
        m_MaterialStack.pop();

        #ifdef NO_INSTANCED
            if (!m_MaterialStack.empty())
            {
                ResourcePtr<Material> material = m_MaterialStack.top();
                material->Bind();
            }
        #endif
    }
}
