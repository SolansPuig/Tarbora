#pragma once
#include "../../Framework/Global.hpp"

namespace Tarbora {
    class NodeProperty;
    typedef std::shared_ptr<NodeProperty> PropertyPtr;

    class NodeProperty {
    public:
        NodeProperty(const glm::vec3 &value)
            : m_Value(value) {}

        virtual void Update(float deltaTime) {}

        virtual void Set(const glm::vec3 &newValue);
        virtual void Add(const glm::vec3 &newValue);
        virtual void InterpolateTo(const glm::vec3 &targetValue, float timeToComplete) {}
        const glm::vec3 &Get();

        virtual bool IsAnimatable() { return false; }
        virtual PropertyPtr MakeAnimatable();

    protected:
        glm::vec3 m_Value;
    };

    class AnimatedNodeProperty : public NodeProperty {
        friend class NodeProperty;
    public:
        virtual void Update(float deltaTime);
        virtual void InterpolateTo(const glm::vec3 &targetValue, float timeToComplete);

        virtual bool IsAnimatable() { return true; }
        virtual PropertyPtr MakeAnimatable() { return PropertyPtr(this); }

    protected:
        AnimatedNodeProperty(const glm::vec3 &value)
            : NodeProperty(value), m_TargetTime(0.0f), m_OldValue(value), m_TargetValue(value) {}

        virtual void Interpolate(float fraction);

        float m_ElapsedTime;
        float m_TargetTime;
        glm::vec3 m_OldValue;
        glm::vec3 m_TargetValue;
    };

    class Position : public NodeProperty {
    public:
        Position(const glm::vec3 &value, glm::mat4 *transformation)
            : NodeProperty(value), m_Transformation(transformation) {}

        virtual void Add(const glm::vec3 &value);

        virtual PropertyPtr MakeAnimatable();

    protected:
        glm::mat4 *m_Transformation;
    };

    class AnimatedPosition : public AnimatedNodeProperty {
        friend class Position;
    public:
        virtual void Add(const glm::vec3 &value);

    protected:
        AnimatedPosition(const glm::vec3 &value, glm::mat4 *transformation)
            : AnimatedNodeProperty(value), m_Transformation(transformation) {}

        glm::mat4 *m_Transformation;
    };

    class Rotation : public NodeProperty {
    public:
        Rotation(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : NodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        virtual void Add(const glm::vec3 &value);

        virtual PropertyPtr MakeAnimatable();

    protected:
        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };

    class AnimatedRotation : public AnimatedNodeProperty {
        friend class Rotation;
    public:
        virtual void Add(const glm::vec3 &value);

    protected:
        AnimatedRotation(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : AnimatedNodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        virtual void Interpolate(float fraction);

        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };

    class Scale : public NodeProperty {
    public:
        Scale(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : NodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        virtual void Set(const glm::vec3 &value);
        virtual void Add(const glm::vec3 &value);

        virtual PropertyPtr MakeAnimatable();

    protected:
        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };

    class AnimatedScale : public AnimatedNodeProperty {
        friend class Scale;
    public:
        virtual void Set(const glm::vec3 &value);
        virtual void Add(const glm::vec3 &value);

    protected:
        AnimatedScale(const glm::vec3 &value, glm::mat4 *transformation, glm::vec3 *origin)
            : AnimatedNodeProperty(value), m_Transformation(transformation), m_Origin(origin) {}

        glm::mat4 *m_Transformation;
        glm::vec3 *m_Origin;
    };
}
