#include "../inc/AnimationController.hpp"
#include "../inc/ActorModel.hpp"

namespace Tarbora {
    AnimationController::AnimationController(ActorModel *actor, std::string animations_file) :
        m_ActorModel(actor), m_AnimationsFile("animations/" + animations_file)
    {
        // m_Counter = 0.0f;
        // m_NextAnimationFrame = 0.0f;
        // m_AnimationDuration = 0.0f;
        // m_BlendTime = 0.0f;
    }

    AnimationController::~AnimationController()
    {

    }

    void AnimationController::Update(float deltaTime)
    {
        // if (m_BlendTime <= 0.0f)
        // {
        //     if (m_Counter == 0.0f || (m_Counter >= m_NextAnimationFrame && m_NextAnimationFrame != 0.0f))
        //     {
        //         UpdateAnimation(m_NextAnimationFrame);
        //     }
        //
        //     m_Counter += deltaTime;
        //     if (m_Counter >= m_AnimationDuration)
        //         m_Counter = 0.0f;
        // }
        // else
        // {
        //     m_BlendTime -= deltaTime;
        // }
    }

    void AnimationController::SetAnimation(std::string animation_name)
    {
        // LOG_DEBUG("Setting animation %s", animation_name.c_str());
        // JsonPtr resource = GET_RESOURCE(Json, m_AnimationsFile);
        //
        // raw_json animation;
        // resource->Get(animation_name.c_str(), &animation, {true});
        // resource->PushErrName(animation_name);
        //
        // resource->Get(animation, "duration", &m_AnimationDuration, {true, true});
        // resource->Get(animation, "blendTime", &m_BlendTime, {true, true});
        //
        // raw_json nodes;
        // resource->Get(animation, "nodes", &nodes, {true});
        //
        // m_CurrentAnimationName = animation_name;
        // m_CurrentAnimation = nodes;
        //
        // UpdateAnimation(-1);
        //
        // resource->PopErrName();
    }

    inline float ParseAnimationValue(raw_json newValue, float currentValue, float modifier=1.0f)
    {
        // if (newValue.is_number())
        //     return (float)(newValue)/modifier;
        // else if(newValue.is_string())
        // {
        //     std::string value = newValue;
        //     if (value[0] == '~')
        //     {
        //         if (value == "~")
        //             return currentValue;
        //         else
        //             return currentValue + stof(value.erase(0, 1))/modifier;
        //     }
        // }
        //
        // return currentValue;
    }

    void AnimationController::UpdateAnimation(float frame)
    {
        // m_NextAnimationFrame = 0.0f;
        //
        // JsonPtr resource = GET_RESOURCE(Json, m_AnimationsFile);
        // resource->PushErrName(m_CurrentAnimationName);
        // resource->PushErrName("nodes");
        //
        // for (auto &nodeName : m_CurrentAnimation.items())
        // {
        //     resource->PushErrName(nodeName.key());
        //
        //     std::shared_ptr<SceneNode> node = m_ActorModel->m_Nodes[nodeName.key()];
        //     float pixelDensity = m_ActorModel->m_PixelDensity;
        //
        //     for (auto &transform : nodeName.value().items())
        //     {
        //         resource->PushErrName(transform.key());
        //
        //         if (transform.key() == "rotation")
        //         {
        //             if (transform.value().is_array() && frame < 0.0f)
        //             {
        //                 glm::vec3 rotation;
        //                 rotation.x = ParseAnimationValue(transform.value()[0], node->GetRotation().x);
        //                 rotation.y = ParseAnimationValue(transform.value()[1], node->GetRotation().y);
        //                 rotation.z = ParseAnimationValue(transform.value()[2], node->GetRotation().z);
        //                 node->RotateTo(rotation, m_BlendTime);
        //             }
        //             else if (transform.value().is_object())
        //             {
        //                 glm::vec3 nextRotation;
        //                 float stepTime = 0.0f;
        //                 for (auto &step : transform.value().items())
        //                 {
        //                     resource->PushErrName(step.key());
        //                     stepTime = stof(step.key());
        //                     if (stepTime > frame)
        //                     {
        //                         nextRotation.x = ParseAnimationValue(step.value()[0], node->GetRotation().x);
        //                         nextRotation.y = ParseAnimationValue(step.value()[1], node->GetRotation().y);
        //                         nextRotation.z = ParseAnimationValue(step.value()[2], node->GetRotation().z);
        //
        //                         if (m_NextAnimationFrame == 0.0f || stepTime < m_NextAnimationFrame)
        //                             m_NextAnimationFrame = stepTime;
        //
        //                         break;
        //                     }
        //                     else if (stepTime == 0.0f)
        //                     {
        //                         nextRotation.x = ParseAnimationValue(step.value()[0], node->GetRotation().x);
        //                         nextRotation.y = ParseAnimationValue(step.value()[1], node->GetRotation().y);
        //                         nextRotation.z = ParseAnimationValue(step.value()[2], node->GetRotation().z);
        //                     }
        //                     else
        //                     {
        //                         stepTime = 0.0f;
        //                     }
        //                 }
        //                 float timeToAnimate = ((stepTime > 0.0f) ? (stepTime - frame) : (m_AnimationDuration - frame));
        //                 node->RotateTo(nextRotation, ((frame >= 0.0f) ? timeToAnimate : m_BlendTime));
        //             }
        //         }
        //         else if (transform.key() == "position")
        //             {
        //                 if (transform.value().is_array() && frame < 0.0f)
        //                 {
        //                     glm::vec3 position;
        //                     position.x = ParseAnimationValue(transform.value()[0], node->GetPosition().x, pixelDensity);
        //                     position.y = ParseAnimationValue(transform.value()[1], node->GetPosition().y, pixelDensity);
        //                     position.z = ParseAnimationValue(transform.value()[2], node->GetPosition().z, pixelDensity);
        //                     node->SetPosition(position);
        //                 }
        //                 else if (transform.value().is_object())
        //                 {
        //                     glm::vec3 nextPosition;
        //                     float stepTime = 0.0f;
        //                     for (auto &step : transform.value().items())
        //                     {
        //                         resource->PushErrName(step.key());
        //                         stepTime = stof(step.key());
        //                         if (stepTime > frame)
        //                         {
        //                             nextPosition.x = ParseAnimationValue(step.value()[0], node->GetPosition().x, pixelDensity);
        //                             nextPosition.y = ParseAnimationValue(step.value()[1], node->GetPosition().y, pixelDensity);
        //                             nextPosition.z = ParseAnimationValue(step.value()[2], node->GetPosition().z, pixelDensity);
        //
        //                             if (m_NextAnimationFrame == 0.0f || stepTime < m_NextAnimationFrame)
        //                                 m_NextAnimationFrame = stepTime;
        //
        //                             break;
        //                         }
        //                         else if (stepTime == 0.0f)
        //                         {
        //                             nextPosition.x = ParseAnimationValue(step.value()[0], node->GetPosition().x, pixelDensity);
        //                             nextPosition.y = ParseAnimationValue(step.value()[1], node->GetPosition().y, pixelDensity);
        //                             nextPosition.z = ParseAnimationValue(step.value()[2], node->GetPosition().z, pixelDensity);
        //                         }
        //                         else
        //                         {
        //                             stepTime = 0.0f;
        //                         }
        //                     }
        //                     float timeToAnimate = ((stepTime > 0.0f) ? (stepTime - frame) : (m_AnimationDuration - frame));
        //                     node->MoveTo(nextPosition, ((frame >= 0.0f) ? timeToAnimate : m_BlendTime));
        //                 }
        //             }
        //         else if (transform.key() == "scale")
        //         {
        //             if (transform.value().is_array() && frame < 0.0f)
        //             {
        //                 glm::vec3 scale;
        //                 scale.x = ParseAnimationValue(transform.value()[0], node->GetScale().x, pixelDensity);
        //                 scale.y = ParseAnimationValue(transform.value()[1], node->GetScale().y, pixelDensity);
        //                 scale.z = ParseAnimationValue(transform.value()[2], node->GetScale().z, pixelDensity);
        //                 node->SetScale(scale);
        //                 // else if (transform.value().is_object())
        //                 // {
        //                 //
        //                 // }
        //             }
        //         }
        //         resource->PopErrName();
        //     }
        //     resource->PopErrName();
        // }
        // resource->PopErrName();
        // resource->PopErrName();
    }
}
