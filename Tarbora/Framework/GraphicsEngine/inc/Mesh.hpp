#pragma once
#include "../../ResourceManager/inc/Resource.hpp"

namespace Tarbora {
    class Mesh : public Resource
    {
    public:
        Mesh(std::string name, unsigned int id, int vertices) : Resource(name), m_Id(id), m_Vertices(vertices) {}
        ~Mesh();
        unsigned int GetId() const { return m_Id; }
        int GetVertices() const { return m_Vertices; }
    protected:
        unsigned int m_Id;
        int m_Vertices;
    };

    class MeshResourceLoader : public ResourceLoader
    {
    public:
        virtual const std::string GetPattern() override { return "*.mesh"; };
        virtual ResourcePtr Load(std::string path) override;
    };
}
