#include "NodeProperty.hpp"

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

    PropertyPtr NodeProperty::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedNodeProperty(m_Value));
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

    PropertyPtr Position::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedPosition(m_Value, m_Transformation));
    }

    void AnimatedPosition::Add(const glm::vec3 &newValue)
    {
        m_Value += newValue;
        *m_Transformation = glm::translate(*m_Transformation, newValue);
    }

    void Rotation::Add(const glm::vec3 &newValue)
    {
        glm::quat addValue(glm::radians(newValue));

        *m_Transformation = glm::translate(*m_Transformation, *m_Origin);
        *m_Transformation = *m_Transformation * glm::mat4_cast(addValue);
        *m_Transformation = glm::translate(*m_Transformation, -(*m_Origin));

        m_Value = glm::degrees(glm::eulerAngles(glm::quat_cast(*m_Transformation)));
    }

    PropertyPtr Rotation::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedRotation(m_Value, m_Transformation, m_Origin));
    }

    void AnimatedRotation::Add(const glm::vec3 &newValue)
    {
        glm::quat addValue(glm::radians(newValue));

        *m_Transformation = glm::translate(*m_Transformation, *m_Origin);
        *m_Transformation = *m_Transformation * glm::mat4_cast(addValue);
        *m_Transformation = glm::translate(*m_Transformation, -(*m_Origin));

        m_Value = glm::degrees(glm::eulerAngles(glm::quat_cast(*m_Transformation)));
    }

    void AnimatedRotation::Interpolate(float fraction)
    {
        if (fraction >= 1.0f)
        {
            Set(m_TargetValue);
            m_TargetTime = 0.0f;
        }
        else
        {
            glm::quat old(glm::radians(m_OldValue));
            glm::quat target(glm::radians(m_TargetValue));

            Set(glm::degrees(glm::eulerAngles(mix(old, target, fraction))));
        }
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

    PropertyPtr Scale::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedScale(m_Value, m_Transformation, m_Origin));
    }


    void AnimatedScale::Set(const glm::vec3 &newValue)
    {
        Add(newValue/m_Value);
    }

    void AnimatedScale::Add(const glm::vec3 &newValue)
    {
        *m_Transformation = glm::scale(*m_Transformation, newValue);
        m_Value *= newValue;
        (*m_Origin) *= newValue;
    }
}
