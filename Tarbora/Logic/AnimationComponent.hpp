#pragma once
#include "Component.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    class AnimationComponent : public Component
    {
        friend class AnimationSystem;
    public:
        AnimationComponent(System *s, const ActorId &id, const LuaTable &table);

        void event(const std::string &event_name);

    private:
        void parseEvent(const std::string &current_animation, LuaTable event);

        std::string animation_controller_path_;
        std::set<std::string> current_animations_;
    };

    class AnimationSystem : public SystemImpl<AnimationComponent>
    {
    public:
        AnimationSystem(World *w) : SystemImpl<AnimationComponent>(w) {}

        static std::string getName() { return "animation"; }

        virtual void init(const ActorId &id);
    };
}
