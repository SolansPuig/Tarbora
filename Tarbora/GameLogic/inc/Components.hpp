#pragma once
#include "Actor.hpp"

#define TypeId 1
#define RenderId 2

namespace Tarbora {
    class TypeComponent : public ActorComponent
    {
    public:
        TypeComponent() {}
        ~TypeComponent() {}

        bool Init(JsonPtr resource, json data);
        ComponentId GetId() const { return TypeId; }

        bool HasType(std::string type) { return count(m_Types.begin(), m_Types.end(), type) > 0; }

        static ActorComponentPtr Creator() { return ActorComponentPtr(new TypeComponent()); }
    private:
        std::vector<std::string> m_Types;
    };

    class ModelComponent : public ActorComponent
    {
    public:
        ModelComponent() {}
        ~ModelComponent() {}

        bool Init(JsonPtr resource, json data);
        void AfterInit();
        ComponentId GetId() const { return RenderId; }

        void SetRenderPass(int renderPass) { m_RenderPass = renderPass; }
        void SetModel(std::string model) { m_Model = model; }
        void SetTexture(std::string texture) { m_Texture = texture; }
        void SetShader(std::string shader) { m_Shader = shader; }
        int GetRenderPass() { return m_RenderPass; }
        std::string GetModel() { return m_Model; }
        std::string GetTexture() { return m_Texture; }
        std::string GetShader() { return m_Shader; }

        static ActorComponentPtr Creator() { return ActorComponentPtr(new ModelComponent()); }
    private:
        int m_RenderPass;
        std::string m_Model, m_Texture, m_Shader;
    };
}
