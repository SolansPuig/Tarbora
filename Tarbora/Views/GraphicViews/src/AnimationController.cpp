#include "../inc/AnimationController.hpp"
#include "../inc/ActorModel.hpp"

namespace Tarbora {
    AnimationController::AnimationController(ActorModel *actor, std::string animations_file) :
        m_ActorModel(actor), m_AnimationsFile("animations/" + animations_file)
    {
        m_Counter = 0.0f;
        m_NextAnimationFrame = 0.0f;
        m_AnimationDuration = 0.0f;
        m_BlendTime = 0.0f;
    }

    AnimationController::~AnimationController()
    {

    }

    void AnimationController::Update(float deltaTime)
    {
        if (m_BlendTime <= 0.0f)
        {
            if (m_Counter == 0.0f || (m_Counter >= m_NextAnimationFrame && m_NextAnimationFrame != 0.0f))
            {
                UpdateAnimation(m_NextAnimationFrame);
            }

            m_Counter += deltaTime;
            if (m_Counter >= m_AnimationDuration)
                m_Counter = 0.0f;
        }
        else
        {
            m_BlendTime -= deltaTime;
        }
    }

    void AnimationController::SetAnimation(std::string animation_name)
    {
        LOG_DEBUG("Setting animation %s", animation_name.c_str());
        JsonPtr resource = GET_RESOURCE(Json, m_AnimationsFile);

        raw_json animation;
        resource->Get(animation_name.c_str(), &animation, {true});
        resource->PushErrName(animation_name);

        resource->Get(animation, "duration", &m_AnimationDuration, {true, true});
        resource->Get(animation, "blendTime", &m_BlendTime, {true, true});

        raw_json nodes;
        resource->Get(animation, "nodes", &nodes, {true});

        m_CurrentAnimationName = animation_name;
        m_CurrentAnimation = nodes;

        UpdateAnimation(-1);

        resource->PopErrName();
    }

    inline float ParseAnimationValue(raw_json newValue, float currentValue)
    {
        if (newValue.is_number())
            return newValue;
        else if(newValue.is_string())
        {
            std::string value = newValue;
            if (value[0] == '~')
            {
                if (value == "~")
                    return currentValue;
                else
                    return currentValue + stof(value.erase(0, 1));
            }
        }

        return currentValue;
    }

    void AnimationController::UpdateAnimation(float frame)
    {
        m_NextAnimationFrame = 0.0f;

        JsonPtr resource = GET_RESOURCE(Json, m_AnimationsFile);
        resource->PushErrName(m_CurrentAnimationName);
        resource->PushErrName("nodes");

        for (auto &nodeName : m_CurrentAnimation.items())
        {
            resource->PushErrName(nodeName.key());

            std::shared_ptr<SceneNode> node = m_ActorModel->m_Nodes[nodeName.key()];

            for (auto &transform : nodeName.value().items())
            {
                std::string propertyName = transform.key();

                resource->PushErrName(propertyName);

                if (transform.value().is_array() && frame < 0.0f)
                {
                    glm::vec3 value;
                    value.x = ParseAnimationValue(transform.value()[0], node->Get(propertyName).x);
                    value.y = ParseAnimationValue(transform.value()[1], node->Get(propertyName).y);
                    value.z = ParseAnimationValue(transform.value()[2], node->Get(propertyName).z);
                    node->InterpolateTo(propertyName, value, m_BlendTime);
                }
                else if (transform.value().is_object())
                {
                    glm::vec3 value;
                    float stepTime = 0.0f;
                    for (auto &step : transform.value().items())
                    {
                        resource->PushErrName(step.key());
                        stepTime = stof(step.key());
                        if (stepTime > frame)
                        {
                            value.x = ParseAnimationValue(step.value()[0], node->Get(propertyName).x);
                            value.y = ParseAnimationValue(step.value()[1], node->Get(propertyName).y);
                            value.z = ParseAnimationValue(step.value()[2], node->Get(propertyName).z);

                            if (m_NextAnimationFrame == 0.0f || stepTime < m_NextAnimationFrame)
                                m_NextAnimationFrame = stepTime;

                            break;
                        }
                        else if (stepTime == 0.0f)
                        {
                            value.x = ParseAnimationValue(step.value()[0], node->Get(propertyName).x);
                            value.y = ParseAnimationValue(step.value()[1], node->Get(propertyName).y);
                            value.z = ParseAnimationValue(step.value()[2], node->Get(propertyName).z);
                        }
                        else
                        {
                            stepTime = 0.0f;
                        }
                        resource->PopErrName();
                    }
                    float timeToAnimate = ((stepTime > 0.0f) ? (stepTime - frame) : (m_AnimationDuration - frame));
                    node->InterpolateTo(propertyName, value, ((frame >= 0.0f) ? timeToAnimate : m_BlendTime));
                }
                resource->PopErrName();
            }
            resource->PopErrName();
        }
        resource->PopErrName();
        resource->PopErrName();
    }
}
