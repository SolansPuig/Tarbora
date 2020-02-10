#pragma once
#include "../../../Framework/Global.hpp"

namespace Tarbora {
    typedef unsigned int MeshId;

    struct RenderElementData
    {
        glm::mat4 transform;
        glm::tvec2<unsigned short> uvMap;
        glm::vec3 meshSize;
        glm::vec3 textureSize;
        glm::tvec3<char> colorPrimary;
        glm::tvec3<char> colorSecondary;
        glm::tvec3<char> colorDetail;
        glm::tvec3<char> colorDetail2;
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
