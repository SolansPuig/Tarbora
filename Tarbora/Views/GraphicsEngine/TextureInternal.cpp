#include "TextureInternal.hpp"

namespace Tarbora {
    TextureInternal::TextureInternal(int width, int height, int nrComponents, void *data)
    {
        m_Width = width;
        m_Height = height;

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
