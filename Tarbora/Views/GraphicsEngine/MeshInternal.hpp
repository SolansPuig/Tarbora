#pragma once
#include "../../Framework/Global.hpp"

namespace Tarbora {
    typedef unsigned int MeshId;

    struct RenderElementData
    {
        glm::mat4 transform;
        glm::vec2 uvMap;
        glm::vec3 textureSize;
        glm::vec3 colorPrimary;
        glm::vec3 colorSecondary;
        glm::vec3 colorDetail;
        glm::vec3 colorDetail2;
    };

    class MeshInternal
    {
    public:
        MeshInternal(std::vector<float> data, unsigned int vertices);
        ~MeshInternal();

        void Bind();
        void Draw();
        void AddInstance(RenderElementData data);
        void DrawInstanced();

        MeshId GetId() { return m_Vao; }
        int GetVertices() const { return m_Vertices; }

    private:
        void Build(std::vector<float> data);
        void Delete();

        unsigned int m_Vao;
        unsigned int m_DataBuffer;
        int m_Vertices;
        std::vector<RenderElementData> m_InstanceData;
    };
}
