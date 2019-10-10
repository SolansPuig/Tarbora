#pragma once
#include "../../../Framework/ResourceManager/inc/Resource.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

namespace Tarbora {
    class Material : public Resource
    {
        friend class MaterialResourceLoader;
    public:
        std::shared_ptr<Texture> GetTexture() { return m_Texture; }
        std::shared_ptr<Shader> GetShader() { return m_Shader; }

    private:
        Material(Module *m, std::string name)
            : Resource(m, name) {}

        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<Shader> m_Shader;
    };

    //! \cond HIDDEN_SYMBOLS
    class MaterialResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.mat.json"; };
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
