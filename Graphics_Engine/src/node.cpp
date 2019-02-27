#include "node.hpp"
#include "iostream"

unsigned int Mesh::Get(std::string shape) {
    assert(shape == "PLANE" || shape == "M_PLANE" || shape == "M_CUBE" || shape == "CUBE");

    if (m_VAOs.count(shape) > 0) return m_VAOs[shape];

    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (shape == "CUBE") {
        float vertices[] = {
            // positions          // normals           // texture coords
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  2.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  2.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  2.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  2.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  2.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  3.0f,  2.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  4.0f,  2.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  1.0f,

             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  1.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    } else if (shape == "M_CUBE") {
        float vertices[] = {
            // positions          // normals           // texture coords
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  2.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  2.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  3.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  4.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  4.0f,  2.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  4.0f,  2.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  2.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  3.0f,  1.0f,

             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  2.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  2.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  3.0f,  1.0f,

             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  2.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,

        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    } else if (shape == "PLANE") {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  2.0f,
             0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  2.0f,

             0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
             0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,
            -0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    } else if (shape == "M_PLANE") {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,
             0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  2.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  1.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,

             0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,
            -0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  2.0f,  1.0f,
             0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    m_VAOs[shape] = VAO;
    return VAO;
}

static Mesh MeshLoader;

Node::Node() {
    Reset();
}

void Node::Reset() {
    m_local_scale = glm::mat4(1.0f);
    m_local_transform = glm::mat4(1.0f);
    m_origin = glm::vec3(0, 0, 0);
    m_active = false;

    for (int i = 0; i < N_CHILDREN; i++) {
        m_children[i] = nullptr;
    }
}

void Node::SetShape(std::string shape) {
    m_mesh = MeshLoader.Get(shape);
    if (shape == "PLANE" || shape == "M_PLANE") m_vertices = 12;
    else if (shape == "CUBE" || shape == "M_CUBE") m_vertices = 36;
}

void Node::AddChild(Node* node, const char* name) {
    int i = 0;
    while(m_children[i] != nullptr) {
        i++;
    }
    strcpy(m_names[i], name);
    m_children[i] = node;
}

Node* Node::GetChild(const char* name) {
    int i = 0;
    while(strcmp(m_names[i], name) != 0) {
        i++;
    }
    return m_children[i];
}

void Node::Draw(Shader* shader) {
    if (m_active) {
        // Update the global transformation matrix
        shader->SetMat4("model", m_local_transform * m_local_scale);
        // Give the shaders the texture information
        shader->SetFloatVec3("size", m_size);
        shader->SetFloatVec2("uv", m_uv);
        // Draw the mesh
        glBindVertexArray(m_mesh);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices);
        // Draw the cildren
        int i = 0;
        while(m_children[i] != nullptr) {
            m_children[i]->Draw(shader, &m_local_transform);
            i++;
        }
    }
}

void Node::Draw(Shader* shader, glm::mat4 * global_transform) {
    if (m_active) {
        // Update the global transformation matrix
        glm::mat4 new_global_transform = *global_transform * m_local_transform;
        shader->SetMat4("model", new_global_transform * m_local_scale);
        // Give the shaders the texture information
        shader->SetFloatVec3("size", m_size);
        shader->SetFloatVec2("uv", m_uv);
        // Draw the mesh
        glBindVertexArray(m_mesh);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices);
        // Draw the cildren
        int i = 0;
        while(m_children[i] != nullptr) {
            m_children[i]->Draw(shader, &new_global_transform);
            i++;
        }
    }
}

void Node::SetUV(float x, float y, float z, float u, float v) {
    m_size = glm::vec3(x, y, z);
    m_uv = glm::vec2(u, v);
}

void Node::SetOrigin(float x, float y, float z) {
    m_origin = glm::vec3(x, y, z);
}

void Node::Rotate(float a, float x, float y, float z) {
    m_local_transform = glm::translate(m_local_transform, m_origin);
    m_local_transform = glm::rotate(m_local_transform, glm::radians(a), glm::vec3(x, y, z));
    m_local_transform = glm::translate(m_local_transform, -m_origin);
}

void Node::Scale(float x, float y, float z) {
    m_origin.x *=  x;
    m_origin.y *=  y;
    m_origin.z *=  z;
    m_local_scale = glm::scale(m_local_scale, glm::vec3(x, y, z));
}

void Node::Scale(float s) {
    m_origin.x *=  s;
    m_origin.y *=  s;
    m_origin.z *=  s;
    m_local_transform = glm::scale(m_local_transform, glm::vec3(s, s, s));
}

void Node::Translate(float x, float y, float z) {
    m_local_transform = glm::translate(m_local_transform, glm::vec3(x, y, z));
}

NodePool::NodePool() {}

Node* NodePool::Use() {
    int i = 0;
    while (m_nodes[i].m_active) {
        assert(i != N_NODES);
        i++;
    }

    Node* new_node = &m_nodes[i];
    new_node->m_active = true;

    return new_node;
}

void NodePool::Release(Node* node) {
    int i = 0;
    while(node->m_children[i] != nullptr) {
        Release(node->m_children[i]);
        i++;
    }

    node->Reset();
}
