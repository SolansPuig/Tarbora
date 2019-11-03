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

    void MeshInternal::AddInstance(RenderElementData data)
    {
        m_InstanceData.push_back(data);
    }

    void MeshInternal::Draw()
    {
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices);
    }

    void MeshInternal::DrawInstanced()
    {
        unsigned int amount = m_InstanceData.size();
        glBindBuffer(GL_ARRAY_BUFFER, m_DataBuffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(RenderElementData), &m_InstanceData[0], GL_STATIC_DRAW);
        glBindVertexArray(m_Vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, m_Vertices, amount);
        m_InstanceData.clear();
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

        glGenBuffers(1, &m_DataBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_DataBuffer);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(12 * sizeof(float)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(16 * sizeof(float)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(18 * sizeof(float)));
        glEnableVertexAttribArray(10);
        glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(21 * sizeof(float)));
        glEnableVertexAttribArray(11);
        glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(24 * sizeof(float)));
        glEnableVertexAttribArray(12);
        glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(27 * sizeof(float)));
        glEnableVertexAttribArray(13);
        glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, 33 * sizeof(float), (void*)(30 * sizeof(float)));

        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);
        glVertexAttribDivisor(11, 1);
        glVertexAttribDivisor(12, 1);
        glVertexAttribDivisor(13, 1);
    }

    void MeshInternal::Delete()
    {
        glDeleteVertexArrays(1, &m_Vao);
    }
}
