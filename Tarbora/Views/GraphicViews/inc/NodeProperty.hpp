#pragma once
#include "../../../Framework/Global.hpp"

namespace Tarbora {
    class NodeProperty {
    public:
        NodeProperty(const glm::vec3 &value)
            : m_Value(value) {}

        virtual void Update(float deltaTime) {}

        virtual void Set(const glm::vec3 &newValue);
        virtual void Add(const glm::vec3 &newValue);
        virtual void InterpolateTo(const glm::vec3 &targetValue, float timeToComplete) {}
        const glm::vec3 &Get();

    protected:
        glm::vec3 m_Value;
    };
    typedef std::shared_ptr<NodeProperty> PropertyPtr;

    class AnimatedNodeProperty : public NodeProperty {
    public:
        AnimatedNodeProperty(const glm::vec3 &value)
            : NodeProperty(value), m_OldValue(value), m_TargetValue(value) {}

        virtual void Update(float deltaTime);
        virtual void InterpolateTo(const glm::vec3 &targetValue, float timeToComplete);

    protected:
        void Interpolate(float fraction);

        float m_ElapsedTime;
        float m_TargetTime;
        glm::vec3 m_OldValue;
        glm::vec3 m_TargetValue;
    };

    class Position : public AnimatedNodeProperty {
    public:
        Position(const glm::vec3 &value, glm::mat4 *transformation)
            : AnimatedNodeProperty(value), m_Transformation(transformation) {}

        virtual void Add(const glm::vec3 &value);

    protected:
        glm::mat4 *m_Transformation;
    };

    class Rotation : public AnimatedNodeProperty {
    public:
        Rotation(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : AnimatedNodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        virtual void Add(const glm::vec3 &value);

    protected:
        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };

    class Scale : public AnimatedNodeProperty {
    public:
        Scale(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : AnimatedNodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        virtual void Set(const glm::vec3 &value);
        virtual void Add(const glm::vec3 &value);

    protected:
        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };
}
