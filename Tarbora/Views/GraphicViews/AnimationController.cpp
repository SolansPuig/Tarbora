#include "AnimationController.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
    AnimationController::AnimationController(ActorModel *actor, const std::string &file) :
        m_ActorModel(actor), m_AnimationsFile("animations/" + file)
    {
        m_Counter = 0.0f;
        m_Crono = 0.0f;
    }

    void AnimationController::Update(float deltaTime)
    {
        m_Counter += deltaTime;
        m_Crono += deltaTime;
        if (m_Counter >= 0.032f)
        {
            UpdateAnimation();
            m_Counter = 0.0f;
        }
    }

    void AnimationController::SetAnimation(const std::string &name)
    {
        m_CurrentAnimationName = name;
        LOG_DEBUG("Setting animation %s", name.c_str());
    }

    glm::vec3 AnimationController::ReadValueWithScale(LuaTable &p, LuaTable &value, const glm::vec3 &currentValue, float scale)
    {
        p.Set("time", m_Crono);
        p.Set("currentX", currentValue.x * scale);
        p.Set("currentY", currentValue.y * scale);
        p.Set("currentZ", currentValue.z * scale);

        glm::vec3 vec;
        for (int i = 0; i < 3; i++)
            vec[i] = value.Get<float>(i+1)/scale;
        return vec;
    }

    glm::vec3 AnimationController::ReadValue(LuaTable &p, const std::string &name, LuaTable &value, const glm::vec3 &currentValue)
    {
        if (name == "position" || name == "scale")
        {
            return ReadValueWithScale(p, value, currentValue, 100.0f);
        }
        else if (name == "color_primary" || name == "color_secondary" ||
            name == "color_detail1" || name == "color_detail2")
        {
            return ReadValueWithScale(p, value, currentValue, 255.0f);
        }
        else
        {
            return ReadValueWithScale(p, value, currentValue, 1.0f);
        }
    }

    void AnimationController::UpdateAnimation()
    {
        ResourcePtr<LuaScript> resource(m_AnimationsFile);
        LuaTable animation = resource->Get(m_CurrentAnimationName);
        LuaTable p = resource->Get("g");
        LuaTable currentAnimation = animation.Get("nodes");

        for (auto nodeKeyValue : currentAnimation)
        {
            std::string nodeName = nodeKeyValue.first.GetAs<std::string>();
            LuaTable nodeLua = nodeKeyValue.second.GetAs<LuaTable>();
            std::shared_ptr<SceneNode> node = m_ActorModel->m_Nodes[nodeName];

            for (auto propertyKeyValue : nodeLua)
            {
                std::string propertyName = propertyKeyValue.first.GetAs<std::string>();
                LuaTable value = propertyKeyValue.second.GetAs<LuaTable>();

                glm::vec3 currentValue = node->Get(propertyName);

                glm::vec3 vector = ReadValue(p, propertyName, value, currentValue);
                node->InterpolateTo(propertyName, vector, m_Counter);
            }
        }
    }
}
