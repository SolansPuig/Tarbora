#pragma once
#include "../../Framework/ResourceManager/inc/Resource.hpp"
#include "TextureInternal.hpp"

namespace Tarbora {
    class Texture : public Resource, public TextureInternal
    {
        friend class TextureResourceLoader;

    private:
        Texture(Module *m, std::string name, int width, int height, int nrComponents, void *data)
            : Resource(m, name), TextureInternal(width, height, nrComponents, data) {}
    };

    //! \cond HIDDEN_SYMBOLS
    class TextureResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.png"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };
    //! \endcond
}
