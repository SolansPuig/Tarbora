#pragma once
#include "../../../Framework/ResourceManager/inc/Resource.hpp"
#include "MeshInternal.hpp"

namespace Tarbora {

    class Mesh : public Resource, public MeshInternal
    {
        friend class MeshResourceLoader;
    private:
        Mesh(std::string name, std::vector<float> data, unsigned int vertices)
            : Resource(name), MeshInternal(data, vertices) {}
    };

    //! \cond HIDDEN_SYMBOLS
    class MeshResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.mesh"; };
        virtual std::shared_ptr<Resource> Load(std::string path) override;
    };
    //! \endcond
}
