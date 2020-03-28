#pragma once
#include "../../../Framework/ResourceManager/Resource.hpp"
#include "TextureInternal.hpp"

namespace Tarbora {
    class Texture : public Resource, public TextureInternal
    {
        friend class TextureResourceLoader;

    private:
        Texture(const std::string &name, int width, int height, int components, void *data)
            : Resource(name), TextureInternal(width, height, components, data) {}
    };

    //! \cond HIDDEN_SYMBOLS
    class TextureResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string getPattern() override { return ".png"; };
        virtual std::shared_ptr<Resource> load(const std::string &path) override;
    };
    //! \endcond
}
