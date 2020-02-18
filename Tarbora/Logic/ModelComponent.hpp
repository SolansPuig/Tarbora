#pragma once
#include "Component.hpp"

namespace Tarbora {
    class ModelComponent : public Component
    {
    public:
        ModelComponent(System *s, const ActorId &id, const LuaTable &table);

        void setRenderPass(int render_pass) { render_pass_ = render_pass; }
        void setModel(const std::string &model) { model_ = model; }
        void setMaterial(const std::string &material) { material_ = material; }

        int getRenderPass() { return render_pass_; }
        const std::string& getModel() { return model_; }
        const std::string& getMaterial() { return material_; }

    private:
        int render_pass_;
        std::string model_;
        std::string material_;
    };

    class ModelSystem : public SystemImpl<ModelComponent>
    {
    public:
        ModelSystem(World *w) : SystemImpl<ModelComponent>(w) {}

        static std::string getName() { return "model"; }

        virtual void init(const ActorId &id);
    };
}
