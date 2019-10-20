#pragma once
#include "../../Framework/Global.hpp"

namespace Tarbora {
    typedef unsigned int MeshId;

    class MeshInternal
    {
    public:
        MeshInternal(std::vector<float> data, unsigned int vertices);
        ~MeshInternal();

        void Bind();
        void Draw();

        MeshId GetId() { return m_Vao; }
        int GetVertices() const { return m_Vertices; }

    private:
        void Build(std::vector<float> data);
        void Delete();

        unsigned int m_Vao;
        int m_Vertices;
    };
}
