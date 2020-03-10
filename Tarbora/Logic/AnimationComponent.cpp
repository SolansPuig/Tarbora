#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    AnimationComponent::AnimationComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        animation_controller_path_ = table.get<std::string>("file");
    }

    void AnimationComponent::event(const std::string &event_name)
    {
        ResourcePtr<LuaScript> resource(animation_controller_path_);
        if (resource != nullptr)
        {
            LuaTable controller = resource->get("animation_controller");
            for (auto animation : current_animations_)
            {
                LuaTable event = controller.get(animation).get("events").get(event_name, true);
                if (event.valid())
                {
                    parseEvent(animation, event);
                }
            }
        }
    }

    void AnimationComponent::parseEvent(const std::string &current_animation, LuaTable event)
    {
        LuaTable stop = event.get("stop", true);
        if (stop.valid())
        {
            Message::EndAnimation msg(owner_, current_animation, stop.get<int>("stop_mode", 0, true));
            msg.setFadeOutTimer(stop.get<float>("fade_out", 0.f, true));
            trigger("end_animation", msg);
            current_animations_.erase(current_animation);
        }
        LuaTable play = event.get("play");
        if (play.valid())
        {
            ResourcePtr<LuaScript> resource(animation_controller_path_);
            if (resource != nullptr)
            {
                LuaTable controller = resource->get("animation_controller");
                for (auto anim : play)
                {
                    LuaTable play_anim = anim.second.getAs<LuaTable>();
                    std::string name = play_anim.get<std::string>("name");
                    LuaTable animation = controller.get(name);
                    Message::StartAnimation msg(owner_, name, animation.get<std::string>("file"), play_anim.get<int>("blend_mode", 0, true));
                    msg.setLoop(animation.get<bool>("loop", false, true));
                    msg.setSpeed(animation.get<float>("speed", 1.f, true));
                    msg.setBlendFactor(play_anim.get<float>("blend_factor", 1.f, true));
                    msg.setFadeInTimer(play_anim.get<float>("fade_in", 0.f, true));
                    if (animation.get<bool>("base", false, true))
                    {
                        trigger("start_base_animation", msg);
                    }
                    else
                    {
                        trigger("start_animation", msg);
                    }
                    current_animations_.insert(name);
                }
            }
        }

    }

    void AnimationSystem::init(const ActorId &id)
    {
        AnimationComponent *animation = static_cast<AnimationComponent*>(get(id));
        if (animation && !animation->error())
        {
            ResourcePtr<LuaScript> resource(animation->animation_controller_path_);
            if (resource != nullptr)
            {
                LuaTable start = resource->get("animation_controller").get("start");
                animation->parseEvent("", start);
                animation->current_animations_.erase("start");
                animation->enable();
            }
        }
    }
}
