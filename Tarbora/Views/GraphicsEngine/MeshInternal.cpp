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
        glEnableVertexAttribArray(4); // transform[0]
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(5); // transform[1]
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(6); // transform[2]
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(7); // transform[3]
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(12 * sizeof(float)));
        glEnableVertexAttribArray(8); // UV Map
        glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(16 * sizeof(float)));
        glEnableVertexAttribArray(9); // Mesh size
        glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(18 * sizeof(float)));
        glEnableVertexAttribArray(10); // Texture size
        glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(21 * sizeof(float)));
        glEnableVertexAttribArray(11); // Color primary
        glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(24 * sizeof(float)));
        glEnableVertexAttribArray(12); // Color secondary
        glVertexAttribPointer(12, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(27 * sizeof(float)));
        glEnableVertexAttribArray(13); // Color detail 1
        glVertexAttribPointer(13, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(30 * sizeof(float)));
        glEnableVertexAttribArray(14); // Color detail 2
        glVertexAttribPointer(14, 3, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(33 * sizeof(float)));

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
        glVertexAttribDivisor(14, 1);
    }

    void MeshInternal::Delete()
    {
        glDeleteVertexArrays(1, &m_Vao);
    }
}
