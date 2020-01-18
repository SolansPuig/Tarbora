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

    void AnimationController::UpdateAnimation()
    {
        ResourcePtr<LuaScript> resource(m_AnimationsFile);
        LuaTable animation = resource->Get(m_CurrentAnimationName);
        LuaTable query = resource->Get("query");
        query.Set("time", m_Crono);
        LuaTable currentAnimation = animation.Get("nodes");

        for (auto nodeKeyValue : currentAnimation)
        {
            std::string nodeName = nodeKeyValue.first.GetAs<std::string>();
            LuaTable nodeLua = nodeKeyValue.second.GetAs<LuaTable>();
            std::shared_ptr<SceneNode> node = m_ActorModel->m_Nodes[nodeName];

            for (auto property : nodeLua)
            {
                std::string name = property.first.GetAs<std::string>();
                auto value = property.second;

                glm::vec3 currentValue = node->Get(name);

                float scale = 1.f;
                if (name == "position" || name == "scale") scale = 100.f;
                else if (name.substr(0,5) == "color") scale = 255.f;

                query.Set("currentX", currentValue.x * scale);
                query.Set("currentY", currentValue.y * scale);
                query.Set("currentZ", currentValue.z * scale);

                node->InterpolateTo(name, value.GetAs<glm::vec3>()/scale, m_Counter);
            }
        }
    }
}
