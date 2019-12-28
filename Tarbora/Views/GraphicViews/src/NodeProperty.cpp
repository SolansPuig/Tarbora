#include "../inc/NodeProperty.hpp"

namespace Tarbora {
    void NodeProperty::Set(const glm::vec3 &newValue)
    {
        Add(newValue - m_Value);
    }

    void NodeProperty::Add(const glm::vec3 &newValue)
    {
        m_Value += newValue;
    }

    const glm::vec3 &NodeProperty::Get()
    {
        return m_Value;
    }

    void AnimatedNodeProperty::Update(float deltaTime) {
        if (m_TargetTime > 0.0f)
        {
            m_ElapsedTime += deltaTime;
            Interpolate(m_ElapsedTime/m_TargetTime);
        }
    }

    void AnimatedNodeProperty::InterpolateTo(const glm::vec3 &targetValue, float timeToComplete) {
        if (timeToComplete > 0.0f)
        {
            m_OldValue = m_Value;
            m_TargetValue = targetValue;
            m_ElapsedTime = 0.0f;
            m_TargetTime = timeToComplete;
        } else {
            Set(targetValue);
        }
    }

    void AnimatedNodeProperty::Interpolate(float fraction)
    {
        if (fraction >= 1.0f)
        {
            Set(m_TargetValue);
            m_TargetTime = 0.0f;
        }
        else
        {
            glm::vec3 value = m_OldValue + fraction*(m_TargetValue - m_OldValue);
            Set(value);
        }
    }

    void Position::Add(const glm::vec3 &newValue)
    {
        m_Value += newValue;
        *m_Transformation = glm::translate(*m_Transformation, newValue);
    }

    void Rotation::Add(const glm::vec3 &newValue)
    {
        *m_Transformation = glm::translate(*m_Transformation, *m_Origin);
        if (newValue.x)
        {
            *m_Transformation = glm::rotate(*m_Transformation, glm::radians(newValue.x), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (newValue.y)
        {
            *m_Transformation = glm::rotate(*m_Transformation, glm::radians(newValue.y), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (newValue.z)
        {
            *m_Transformation = glm::rotate(*m_Transformation, glm::radians(newValue.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        *m_Transformation = glm::translate(*m_Transformation, -(*m_Origin));

        m_Value += newValue;
    }

    void Scale::Set(const glm::vec3 &newValue)
    {
        Add(newValue/m_Value);
    }

    void Scale::Add(const glm::vec3 &newValue)
    {
        *m_Transformation = glm::scale(*m_Transformation, newValue);
        m_Value *= newValue;
        (*m_Origin) *= newValue;
    }
}
