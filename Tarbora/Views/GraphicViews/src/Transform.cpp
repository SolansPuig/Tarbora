#include "../inc/Transform.hpp"

Transform::Transform(const glm::mat3 &basis, const glm::vec3 &origin) :
    m_Basis(basis), m_Origin(origin)
{}

Transform::Transform(const glm::vec3 &rotation, const glm::vec3 &origin) :
    m_Origin(origin)
{
    SetRotation(rotation);
}

void Transform::SetTranslation(const glm::vec3 &translation)
{
    for (int i = 0; i < 3; i++)
    {
        m_Origin[i] = glm::dot(m_Basis[i], translation);
    }
}

void Transform::Translate(const glm::vec3 &translation)
{
    for (int i = 0; i < 3; i++)
    {
        m_Origin[i] += glm::dot(m_Basis[i], translation);
    }
}

void Transform::Scale(const glm::vec3 &scale)
{
    for (int i = 0; i < 3; i++)
    {
        m_Basis[i] *= scale[i];
    }
    m_Origin *= scale;
}

void Transform::Scale(float scale)
{
    Scale(glm::vec3(scale, scale, scale));
}

void Transform::SetRotation(const glm::vec3 &rotation)
{
    glm::vec3 axis = glm::normalize(rotation);
    float angle = glm::length(rotation);
    SetRotation(axis, angle);
}

void Transform::SetRotation(const glm::vec3 &axis, float angle)
{
    glm::vec3 axis_sq(axis.x * axis.x, axis.y * axis.y, axis.z * axis.z);
    float cosine = cos(angle);
    float cosineInv = 1 - cosine;
    float sine = sin(angle);

    for (int i = 0; i < 3; i++)
    {
        m_Basis[i][i] = cosine + axis_sq[i]*cosineInv;
    }

    float v1 = axis.x * axis.y * cosineInv;
    float v2 = axis.z * sine;
    m_Basis[0][1] = v1 - v2;
    m_Basis[1][0] = v1 + v2;

    v1 = axis.x * axis.z * cosineInv;
    v2 = axis.y * sine;
    m_Basis[0][2] = v1 + v2;
    m_Basis[2][0] = v1 - v2;

    v1 = axis.y * axis.z * cosineInv;
    v2 = axis.x * sine;
    m_Basis[1][2] = v1 - v2;
    m_Basis[2][1] = v1 + v2;
}

void Transform::Rotate(const glm::vec3 &rotation)
{
    (*this) = Transform(rotation, glm::vec3()) * (*this);
}
