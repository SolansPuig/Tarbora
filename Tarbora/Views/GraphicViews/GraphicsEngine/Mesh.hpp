#pragma once
#include "../../../Framework/ResourceManager/Resource.hpp"
#include "MeshInternal.hpp"

namespace Tarbora {

    class Mesh : public Resource, public MeshInternal
    {
        friend class MeshResourceLoader;
    private:
        Mesh(const std::string &name, const std::vector<float> &data, unsigned int vertices)
            : Resource(name), MeshInternal(data, vertices) {}
    };

    //! \cond HIDDEN_SYMBOLS
    class MeshResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string getPattern() override { return "*.mesh"; };
        virtual std::shared_ptr<Resource> load(const std::string &path) override;
    };
    //! \endcond
}
