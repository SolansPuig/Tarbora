#pragma once
#include "Component.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    class AnimationComponent : public Component
    {
    public:
        AnimationComponent(System *s, const ActorId &id, const LuaTable &table);

        void SetAnimation(const std::string &animation);

        const std::string &GetAnimation() { return m_Animation; }

    private:
        std::string m_File;
        std::string m_Animation;
    };

    class AnimationSystem : public SystemImpl<AnimationComponent>
    {
    public:
        AnimationSystem(World *w) : SystemImpl<AnimationComponent>(w) {}

        static std::string GetName() { return "animation"; }

        virtual void Init(const ActorId &id);
    };
}
