#include "../inc/GraphicsEngine.hpp"
#include "../inc/TextureInternal.hpp"

namespace Tarbora {
    ResourcePtr TextureResourceLoader::Load(std::string path)
    {
        ResourcePtr r = ResourcePtr(new Texture(m_Module, path));
        std::static_pointer_cast<Texture>(r)->Configure(GL_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);
        return r;
    }
}
