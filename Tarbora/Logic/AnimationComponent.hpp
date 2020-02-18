#pragma once
#include "Component.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    class AnimationComponent : public Component
    {
    public:
        AnimationComponent(System *s, const ActorId &id, const LuaTable &table);

        void set(const std::string &animation);

        const std::string& get() { return animation_; }

    private:
        std::string file_;
        std::string animation_;
    };

    class AnimationSystem : public SystemImpl<AnimationComponent>
    {
    public:
        AnimationSystem(World *w) : SystemImpl<AnimationComponent>(w) {}

        static std::string getName() { return "animation"; }

        virtual void init(const ActorId &id);
    };
}
