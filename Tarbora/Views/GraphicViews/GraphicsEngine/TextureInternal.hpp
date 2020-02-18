#pragma once
#include "../../../Framework/Global.hpp"
#include "Window.hpp"

namespace Tarbora {
    class TextureInternal
    {
    public:
        TextureInternal(int width, int height, int components, void *data);
        TextureInternal(int width, int height, unsigned int format, unsigned int type=GL_UNSIGNED_BYTE, void *data=nullptr);
        TextureInternal(int width, int height, unsigned int internalFormat, unsigned int type, unsigned int format, void *data=nullptr);
        ~TextureInternal();

        void configure(unsigned int min_filter, unsigned int mag_filter);
        void configure(unsigned int min_filter, unsigned int mag_filter, unsigned int wrapS, unsigned int wrapT);

        void bind(unsigned int channel=0) const;

        unsigned int getId() const { return id_; }
        int getWidth() const { return width_; }
        int getHeight() const { return height_; }

    private:
        void build(void *data, unsigned int format, unsigned int type);
        void build(void *data, unsigned int internal_format, unsigned int type, unsigned int format);
        void deleteTexture();

        unsigned int id_;
        int width_, height_;
    };
}
