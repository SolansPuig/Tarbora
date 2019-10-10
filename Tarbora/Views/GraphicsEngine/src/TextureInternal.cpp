#include "../inc/TextureInternal.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../Framework/External/stb_image.h"

namespace Tarbora {
    TextureInternal::TextureInternal(std::string name)
    {
        // Read the image file
        int nrComponents;
        void *data = LoadFromFile(name, &nrComponents);

        // Detect the format
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        // Build the texture
        Build(data, format, GL_UNSIGNED_BYTE);

        // Delete the image
        stbi_image_free(data);
    }

    TextureInternal::TextureInternal(int width, int height, unsigned int format, unsigned int type, void *data)
    {
        m_Width = width;
        m_Height = height;

        Build(data, format, type, format);
    }

    TextureInternal::TextureInternal(int width, int height, unsigned int internalFormat, unsigned int type, unsigned int format, void *data)
    {
        m_Width = width;
        m_Height = height;

        Build(data, internalFormat, type, format);
    }

    TextureInternal::~TextureInternal()
    {
        Delete();
    }

    void TextureInternal::Configure(unsigned int minFilter, unsigned int magFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    void TextureInternal::Configure(unsigned int minFilter, unsigned int magFilter, unsigned int wrapS, unsigned int wrapT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }


    void TextureInternal::Bind(unsigned int channel)
    {
        glActiveTexture(GL_TEXTURE0 + channel);
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    void *TextureInternal::LoadFromFile(std::string path, int *nrComponents)
    {
        // Load the raw image
        void *data = stbi_load(path.c_str(), &m_Width, &m_Height, nrComponents, 0);
        if (data == nullptr)
        {
            LOG_ERR("TextureLoader: The image %s failed to load due to: %s", path.c_str(), stbi_failure_reason());
            data = stbi_load("../Resources/textures/missing.png", &m_Width, &m_Height, nrComponents, 0);
            if (data == nullptr)
                LOG_ERR("TextureLoader: The image textures/missing.png failed to load due to: %s", stbi_failure_reason());
        }

        return data;
    }

    void TextureInternal::Build(void *data, unsigned int format, unsigned int type)
    {
        Build(data, format, type, format);
    }

    void TextureInternal::Build(void *data, unsigned int internalFormat, unsigned int type, unsigned int format)
    {
        glGenTextures(1, &m_Id);
        glBindTexture(GL_TEXTURE_2D, m_Id);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void TextureInternal::Delete()
    {
        glDeleteTextures(1, &m_Id);
    }
}
