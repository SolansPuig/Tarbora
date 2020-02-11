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

    void NodePosition::Add(const glm::vec3 &newValue)
    {
        m_Value += newValue;
        *m_Transformation = glm::translate(*m_Transformation, newValue);
    }

    PropertyPtr NodePosition::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedNodePosition(m_Value, m_Transformation));
    }

    void AnimatedNodePosition::Add(const glm::vec3 &newValue)
    {
        m_Value += newValue;
        *m_Transformation = glm::translate(*m_Transformation, newValue);
    }

    void NodeRotation::Add(const glm::vec3 &newValue)
    {
        glm::quat addValue(glm::radians(newValue));

        *m_Transformation = glm::translate(*m_Transformation, *m_Origin);
        *m_Transformation = *m_Transformation * glm::mat4_cast(addValue);
        *m_Transformation = glm::translate(*m_Transformation, -(*m_Origin));

        m_Value = glm::degrees(glm::eulerAngles(glm::quat_cast(*m_Transformation)));
    }

    PropertyPtr NodeRotation::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedNodeRotation(m_Value, m_Transformation, m_Origin));
    }

    void AnimatedNodeRotation::Add(const glm::vec3 &newValue)
    {
        glm::quat addValue(glm::radians(newValue));

        *m_Transformation = glm::translate(*m_Transformation, *m_Origin);
        *m_Transformation = *m_Transformation * glm::mat4_cast(addValue);
        *m_Transformation = glm::translate(*m_Transformation, -(*m_Origin));

        m_Value = glm::degrees(glm::eulerAngles(glm::quat_cast(*m_Transformation)));
    }

    void AnimatedNodeRotation::Interpolate(float fraction)
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

    void NodeScale::Set(const glm::vec3 &newValue)
    {
        Add(newValue/m_Value);
    }

    void NodeScale::Add(const glm::vec3 &newValue)
    {
        *m_Transformation = glm::scale(*m_Transformation, newValue);
        m_Value *= newValue;
        (*m_Origin) *= newValue;
    }

    PropertyPtr NodeScale::MakeAnimatable()
    {
        return PropertyPtr(new AnimatedNodeScale(m_Value, m_Transformation, m_Origin));
    }


    void AnimatedNodeScale::Set(const glm::vec3 &newValue)
    {
        Add(newValue/m_Value);
    }

    void AnimatedNodeScale::Add(const glm::vec3 &newValue)
    {
        *m_Transformation = glm::scale(*m_Transformation, newValue);
        m_Value *= newValue;
        (*m_Origin) *= newValue;
    }
}
