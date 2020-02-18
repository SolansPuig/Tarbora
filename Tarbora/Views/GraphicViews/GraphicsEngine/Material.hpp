#pragma once
#include "Texture.hpp"
#include "Shader.hpp"

namespace Tarbora {
    typedef std::string MaterialId;

    class Material : public Resource
    {
        friend class MaterialResourceLoader;
    public:
        void bind(const glm::mat4 &projection, const glm::mat4 &view) const;

        ResourcePtr<Texture> getAlbedo() const { return albedo_; }
        ResourcePtr<Texture> getSpecular() const { return specular_; }
        ResourcePtr<Texture> getColorTint() const { return color_tint_; }
        ResourcePtr<Shader> getShader() const { return shader_; }
        const MaterialId& getId() const { return name_; }

    private:
        Material(const std::string &name)
            : Resource(name) {}

        ResourcePtr<Texture> albedo_;
        ResourcePtr<Texture> specular_;
        ResourcePtr<Texture> color_tint_;
        ResourcePtr<Shader> shader_;
        int pixel_density_;
    };

    //! \cond HIDDEN_SYMBOLS
    class MaterialResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string getPattern() override { return "*.mat.lua"; };
        virtual std::shared_ptr<Resource> load(const std::string &path) override;
    };
    //! \endcond
}
