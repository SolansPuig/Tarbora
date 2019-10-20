#pragma once
#include "Texture.hpp"
#include "Shader.hpp"

namespace Tarbora {
    typedef std::string MaterialId;

    class Material : public Resource
    {
        friend class MaterialResourceLoader;
    public:
        void Bind();

        ResourcePtr<Texture> GetAlbedo() { return m_Albedo; }
        ResourcePtr<Texture> GetSpecular() { return m_Specular; }
        ResourcePtr<Shader> GetShader() { return m_Shader; }
        MaterialId GetId() { return m_Name; }

    private:
        Material(Module *m, std::string name)
            : Resource(m, name) {}

        ResourcePtr<Texture> m_Albedo;
        ResourcePtr<Texture> m_Specular;
        ResourcePtr<Shader> m_Shader;
    };

    //! \cond HIDDEN_SYMBOLS
    class MaterialResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.mat.json"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };
    //! \endcond
}
