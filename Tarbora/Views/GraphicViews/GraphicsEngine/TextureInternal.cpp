#include "TextureInternal.hpp"

namespace Tarbora {
    TextureInternal::TextureInternal(int width, int height, int components, void *data)
    {
        ZoneScoped;

        width_ = width;
        height_ = height;

        // Detect the format
        GLenum format;
        if (components == 1)
            format = GL_RED;
        else if (components == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        // Build the texture
        build(data, format, GL_UNSIGNED_BYTE);
    }

    TextureInternal::TextureInternal(int width, int height, unsigned int format, unsigned int type, void *data)
    {
        width_ = width;
        height_ = height;

        build(data, format, type, format);
    }

    TextureInternal::TextureInternal(int width, int height, unsigned int internal_format, unsigned int type, unsigned int format, void *data)
    {
        width_ = width;
        height_ = height;

        build(data, internal_format, type, format);
    }

    TextureInternal::~TextureInternal()
    {
        deleteTexture();
    }

    void TextureInternal::configure(unsigned int min_filter, unsigned int mag_filter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    void TextureInternal::configure(unsigned int min_filter, unsigned int mag_filter, unsigned int wrapS, unsigned int wrapT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    }


    void TextureInternal::bind(unsigned int channel) const
    {
        glActiveTexture(GL_TEXTURE0 + channel);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    void TextureInternal::build(void *data, unsigned int format, unsigned int type)
    {
        build(data, format, type, format);
    }

    void TextureInternal::build(void *data, unsigned int internal_format, unsigned int type, unsigned int format)
    {
        ZoneScoped;

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void TextureInternal::deleteTexture()
    {
        glDeleteTextures(1, &id_);
    }
}
