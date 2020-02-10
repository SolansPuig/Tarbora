#include "RenderQueue.hpp"

namespace Tarbora {
    void RenderQueue::DrawMesh(RenderPass renderPass, ResourcePtr<Mesh> mesh, glm::mat4 transform, glm::vec2 uv, glm::vec3 meshSize, glm::vec3 textureSize, glm::vec3 primary, glm::vec3 secondary, glm::vec3 detail, glm::vec3 detail2)
    {
        ResourcePtr<Material> material = m_MaterialStack.top();

        #ifndef NO_INSTANCED
            RenderElementData data;
            data.transform = transform;
            data.uvMap = uv;
            data.meshSize = meshSize;
            data.textureSize = textureSize;
            data.colorPrimary = primary;
            data.colorSecondary = secondary;
            data.colorDetail = detail;
            data.colorDetail2 = detail2;

            m_RenderList[renderPass][material][mesh].emplace_back(data);

        #else
            material->GetShader()->Set("transform", transform);
            material->GetShader()->Set("uv", uv);
            material->GetShader()->Set("meshSize", meshSize);
            material->GetShader()->Set("textureSize", textureSize);
            material->GetShader()->Set("colorPrimary", primary);
            material->GetShader()->Set("colorSecondary", secondary);
            material->GetShader()->Set("colorDetail", detail);
            material->GetShader()->Set("colorDetail2", detail2);
            mesh->Draw();
        #endif
    }

    void RenderQueue::Draw()
    {
        #ifndef NO_INSTANCED
            for (unsigned int pass = RenderPass::Zero; pass < RenderPass::Last; ++pass)
            {
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

                for (auto material : m_RenderList[pass])
                {
                    material.first->Bind(m_Projection, m_View);
                    for (auto mesh : material.second)
                    {
                        mesh.first->Bind();
                        for (auto instance : mesh.second)
                        {
                            mesh.first->AddInstance(instance);
                        }
                        mesh.first->DrawInstanced();
                    }
                }
            }
            m_RenderList.clear();
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

    void RenderQueue::SetProjectionMatrix(const glm::mat4 &m)
    {
        m_Renderer->SetProjectionMatrix(m);
        m_Projection = m;
    }

    void RenderQueue::SetViewMatrix(const glm::mat4 &m)
    {
        m_Renderer->SetViewMatrix(m);
        m_View = m;
    }
}
