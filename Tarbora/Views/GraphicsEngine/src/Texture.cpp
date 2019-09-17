#include "../inc/GraphicsEngine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Framework/External/stb_image.h"
#include "../../GraphicViews/inc/GraphicView.hpp"

#define GAMEVIEW(MODULE) static_cast<GraphicView*>(MODULE)

namespace Tarbora {
    Texture::~Texture()
    {
        GAMEVIEW(m_Module)->GraphicsEngine()->DeleteTexture(m_Id);
    }

    ResourcePtr TextureResourceLoader::Load(std::string path)
    {
        int nrComponents, width, height;

        // Load the raw image
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data == nullptr)
        {
            LOG_ERR("TextureLoader: The image %s failed to load due to: %s", path.c_str(), stbi_failure_reason());
            data = stbi_load("../Resources/textures/missing.png", &width, &height, &nrComponents, 0);
            if (data == nullptr)
                LOG_ERR("TextureLoader: The image textures/missing.png failed to load due to: %s", stbi_failure_reason());
        }

        unsigned int id = GAMEVIEW(m_Module)->GraphicsEngine()->LoadTexture(data, width, height, nrComponents);

        // Delete the image, as it is already on the GPU
        stbi_image_free(data);

        ResourcePtr r = ResourcePtr(new Texture(m_Module, path, id, width, height));
        return r;
    }
}
