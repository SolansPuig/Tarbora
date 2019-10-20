#include "MeshInternal.hpp"
#include "Window.hpp"

namespace Tarbora {
    MeshInternal::MeshInternal(std::vector<float> data, unsigned int vertices)
    {
        m_Vertices = vertices;
        Build(data);
    }

    MeshInternal::~MeshInternal()
    {
        Delete();
    }

    void MeshInternal::Bind()
    {
        glBindVertexArray(m_Vao);
    }

    void MeshInternal::Draw()
    {
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices);
    }

    void MeshInternal::Build(std::vector<float> data)
    {
        unsigned int VBO;
        glGenVertexArrays(1, &m_Vao);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

        glBindVertexArray(m_Vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
    }

    void MeshInternal::Delete()
    {
        glDeleteVertexArrays(1, &m_Vao);
    }
}
