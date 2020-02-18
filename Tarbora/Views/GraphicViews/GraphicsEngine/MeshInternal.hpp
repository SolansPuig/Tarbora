#pragma once
#include "../../../Framework/Global.hpp"

namespace Tarbora {
    typedef unsigned int MeshId;

    struct RenderElementData
    {
        glm::mat4 transform;
        glm::tvec2<unsigned short> uv_map;
        glm::vec3 mesh_size;
        glm::vec3 texture_size;
        glm::tvec3<unsigned char> color_primary;
        glm::tvec3<unsigned char> color_secondary;
        glm::tvec3<unsigned char> color_detail;
        glm::tvec3<unsigned char> color_detail2;
    };

    class MeshInternal
    {
    public:
        MeshInternal(const std::vector<float> &data, unsigned int vertices);
        ~MeshInternal();

        void bind();
        void draw();
        void addInstance(const RenderElementData &data);
        void drawInstanced();

        const MeshId &getId() { return vao_; }
        int getVertices() const { return vertices_; }

    private:
        void build(const std::vector<float> &data);
        void deleteMesh();

        unsigned int vao_;
        unsigned int data_buffer_;
        int vertices_;
        std::vector<RenderElementData> instance_data_;
    };
}
