#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    AnimationComponent::AnimationComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        file_ = table.get<std::string>("file", "");
    }

    void AnimationComponent::set(const std::string &animation)
    {
        if (animation_ != animation)
        {
            animation_ = animation;
            trigger("set_animation", Message::SetAnimation(owner_, animation_, file_));
        }
    }

    void AnimationSystem::init(const ActorId &id)
    {
        AnimationComponent *animation = static_cast<AnimationComponent*>(get(id));
        if (animation && !animation->error())
        {
            animation->enable();
        }
    }
}
