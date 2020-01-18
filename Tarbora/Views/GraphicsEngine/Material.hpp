#pragma once
#include "Texture.hpp"
#include "Shader.hpp"

namespace Tarbora {
    typedef std::string MaterialId;

    class Material : public Resource
    {
        friend class MaterialResourceLoader;
    public:
        void Bind() const;

        ResourcePtr<Texture> GetAlbedo() const { return m_Albedo; }
        ResourcePtr<Texture> GetSpecular() const { return m_Specular; }
        ResourcePtr<Texture> GetColorTint() const { return m_ColorTint; }
        ResourcePtr<Shader> GetShader() const { return m_Shader; }
        MaterialId GetId() const { return m_Name; }

    private:
        Material(Module *m, std::string name)
            : Resource(m, name) {}

        ResourcePtr<Texture> m_Albedo;
        ResourcePtr<Texture> m_Specular;
        ResourcePtr<Texture> m_ColorTint;
        ResourcePtr<Shader> m_Shader;
        int m_PixelDensity;
    };

    //! \cond HIDDEN_SYMBOLS
    class MaterialResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.mat.lua"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };
    //! \endcond
}
