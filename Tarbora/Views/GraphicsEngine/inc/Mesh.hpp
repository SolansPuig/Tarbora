#pragma once
#include "../../../Framework/ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Mesh : public Resource
    {
        friend class MeshResourceLoader;
    public:
        ~Mesh();
        unsigned int GetId() const { return m_Id; }
        int GetVertices() const { return m_Vertices; }
    private:
        Mesh(Module *m, std::string name, unsigned int id, int vertices) : Resource(m, name), m_Id(id), m_Vertices(vertices) {}
        unsigned int m_Id;
        int m_Vertices;
    };

    //! \cond HIDDEN_SYMBOLS
    class MeshResourceLoader : public ResourceLoader
    {
        friend class ResourceManager;
    private:
        virtual const std::string GetPattern() override { return "*.mesh"; };
        virtual ResourcePtr Load(std::string path) override;
    };
    //! \endcond
}
