#pragma once
#include "../../../Framework/ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Texture : public Resource
    {
        friend class TextureResourceLoader;
    public:
        ~Texture();
        unsigned int GetId() const { return m_Id; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
    private:
        Texture(ClientApplication *app, std::string name, unsigned int id, int width, int height)
            : Resource(app, name), m_Id(id), m_Width(width), m_Height(height) {}

        unsigned int m_Id;
        int m_Width, m_Height;
    };

    //! \cond HIDDEN_SYMBOLS
    class TextureResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.png"; };
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
