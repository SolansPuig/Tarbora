#ifndef NODE_HPP
#define NODE_HPP

#include <map>
#include <string>
#include <memory>
#include "shader.hpp"

#define N_NODES 10000
#define N_CHILDREN 16

class Mesh {
public:
    unsigned int Get(std::string shape);
private:
    std::map<std::string, unsigned int> m_VAOs;
};

class Node {
    friend class NodePool;
public:
    void SetShape(std::string shape);

    void AddChild(Node* node, const char* name);
    Node* GetChild(const char* name);

    void Draw(Shader* shader);
    void Draw(Shader* shader, glm::mat4 * global_transform);

    void SetOrigin(float x, float y, float z);
    void SetUV(float x, float y, float z, float u, float v);
    void Rotate(float a, float x, float y, float z);
    void Scale(float x, float y, float z);
    void Scale(float s);
    void Translate(float x, float y, float z);

private:
    Node();
    void Reset();

    unsigned int m_mesh;
    unsigned int m_vertices;
    glm::mat4 m_local_transform;
    glm::mat4 m_local_scale;
    glm::vec3 m_origin;
    glm::vec3 m_size;
    glm::vec2 m_uv;

    char m_names[N_CHILDREN][16];
    Node* m_children[N_CHILDREN];
    bool m_active;
};

class NodePool {
public:
    NodePool();

    Node* Use();
    void Release(Node* node);
private:
    Node m_nodes[N_NODES];
};

#endif
