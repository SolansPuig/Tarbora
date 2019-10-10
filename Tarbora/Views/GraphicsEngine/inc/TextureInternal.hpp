#pragma once
#include "../../../Framework/Global.hpp"
#include "../inc/Window.hpp"

namespace Tarbora {
    class TextureInternal
    {
    public:
        TextureInternal(std::string name);
        TextureInternal(int width, int height, unsigned int format, unsigned int type=GL_UNSIGNED_BYTE, void *data=NULL);
        TextureInternal(int width, int height, unsigned int internalFormat, unsigned int type, unsigned int format, void *data=NULL);
        ~TextureInternal();

        void Configure(unsigned int minFilter, unsigned int magFilter);
        void Configure(unsigned int minFilter, unsigned int magFilter, unsigned int wrapS, unsigned int wrapT);

        void Bind(unsigned int channel=0);

        unsigned int GetId() { return m_Id; }
        int GetWidth() { return m_Width; }
        int GetHeight() { return m_Height; }

    private:
        void *LoadFromFile(std::string path, int *nrComponents);
        void Build(void *data, unsigned int format, unsigned int type);
        void Build(void *data, unsigned int internalFormat, unsigned int type, unsigned int format);
        void Delete();

        unsigned int m_Id;
        int m_Width, m_Height;
    };
}
