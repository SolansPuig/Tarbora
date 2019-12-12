#pragma once

#include "../../../Framework/Global.hpp"

class Transform {
public:
    Transform(const glm::mat3 &basis, const glm::vec3 &origin);
    Transform(const glm::vec3 &rotation, const glm::vec3 &origin);

    void SetTranslation(const glm::vec3 &translation);

    void Translate(const glm::vec3 &translation);

    void Scale(const glm::vec3 &scale);
    void Scale(float scale);

    void SetRotation(const glm::vec3 &rotation);
    void SetRotation(const glm::vec3 &axis, float angle);
    void Rotate(const glm::vec3 &rotation);

    void operator*=(const Transform &transform)
    {
        for (int i = 0; i < 3; i++)
        {
            m_Origin[i] += glm::dot(m_Basis[i], transform.m_Origin);
        }
        m_Basis *= transform.m_Basis;
    }

    Transform operator*(const Transform &transform)
    {
        Transform t = (*this);
        t *= transform;
        return t;
    }

    operator glm::mat4() const {
        glm::mat4 mat(1.0f);
        for (int i = 0; i < 3; i++)
        {
            mat[i] = glm::vec4(m_Basis[i], m_Origin[i]);
        }
        return mat;
    }

private:
    glm::mat3 m_Basis;
    glm::vec3 m_Origin;
};
