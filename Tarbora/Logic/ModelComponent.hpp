#pragma once
#include "Component.hpp"

namespace Tarbora {
    class ModelComponent : public Component
    {
    public:
        ModelComponent(System *s, const ActorId &id, const LuaTable &table);

        void SetRenderPass(int renderPass) { m_RenderPass = renderPass; }
        void SetModel(const std::string &model) { m_Model = model; }
        void SetMaterial(const std::string &material) { m_Material = material; }

        int GetRenderPass() { return m_RenderPass; }
        const std::string &GetModel() { return m_Model; }
        const std::string &GetMaterial() { return m_Material; }

    private:
        int m_RenderPass;
        std::string m_Model;
        std::string m_Material;
    };

    class ModelSystem : public SystemImpl<ModelComponent>
    {
    public:
        ModelSystem(World *w) : SystemImpl<ModelComponent>(w) {}

        static std::string GetName() { return "model"; }

        virtual void Init(const ActorId &id);
    };
}
