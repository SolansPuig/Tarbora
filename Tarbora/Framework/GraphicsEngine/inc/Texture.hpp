#pragma once
#include "../../ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Texture : public Resource
    {
    public:
        Texture(std::string name, unsigned int id, int width, int height)
            : Resource(name), m_Id(id), m_Width(width), m_Height(height) {}
        ~Texture();
        unsigned int GetId() const { return m_Id; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
    protected:
        unsigned int m_Id;
        int m_Width, m_Height;
    };

    class TextureResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.png"; };
        virtual ResourcePtr Load(std::string path) override;
    };
}
