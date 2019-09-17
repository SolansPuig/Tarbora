#pragma once
#include "../../../Framework/Module.hpp"

namespace Tarbora {
    class ActorModel;

    class AnimationController
    {
    public:
        AnimationController(ActorModel *actor, std::string animations_file);
        ~AnimationController();

        void Update(float deltaTime);

        void SetAnimation(std::string animation_name);
        void UpdateAnimation(float frame);

    private:
        ActorModel *m_ActorModel;
        std::string m_AnimationsFile;

        float m_BlendTime;

        float m_Counter;
        float m_NextAnimationFrame;
        float m_AnimationDuration;

        std::string m_CurrentAnimationName;
        raw_json m_CurrentAnimation;
    };
}
