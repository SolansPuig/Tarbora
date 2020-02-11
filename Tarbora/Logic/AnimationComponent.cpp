#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    AnimationComponent::AnimationComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_File = table.Get<std::string>("file", "");
    }

    void AnimationComponent::SetAnimation(const std::string &animation)
    {
        if (m_Animation != animation)
        {
            m_Animation = animation;
            Trigger("set_animation", Message::SetAnimation(m_Owner, m_Animation, m_File));
        }
    }

    void AnimationSystem::Init(const ActorId &id)
    {
        AnimationComponent *animation = static_cast<AnimationComponent*>(Get(id));
        if (animation && !animation->Error())
        {
            animation->Enable();
        }
    }
}
