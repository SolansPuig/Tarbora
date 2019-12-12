#include "GraphicsEngine.hpp"
#include "TextureInternal.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../Framework/External/stb_image.h"

namespace Tarbora {
    std::shared_ptr<Resource> TextureResourceLoader::Load(std::string path)
    {
        // Load the raw image
        int nrComponents, width, height;
        void *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data == nullptr)
        {
            LOG_ERR("TextureLoader: The image %s failed to load due to: %s", path.c_str(), stbi_failure_reason());
            data = stbi_load("../Resources/textures/missing.png", &width, &height, &nrComponents, 0);
            if (data == nullptr)
                LOG_ERR("TextureLoader: The image textures/missing.png failed to load due to: %s", stbi_failure_reason());
                return std::shared_ptr<Resource>();
        }

        std::shared_ptr<Resource> r = std::shared_ptr<Resource>(new Texture(m_Module, path, width, height, nrComponents, data));

        // Delete the image
        stbi_image_free(data);

        // Configure the texture
        std::static_pointer_cast<Texture>(r)->Configure(GL_LINEAR, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        return r;
    }
}
