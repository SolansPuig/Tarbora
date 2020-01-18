#pragma once
#include "../../Framework/Module.hpp"

namespace Tarbora {
    class ActorModel;

    class AnimationController
    {
    public:
        AnimationController(ActorModel *actor, const std::string &file);

        void Update(float deltaTime);

        void SetAnimation(const std::string &name);

    private:
        void UpdateAnimation();

        ActorModel *m_ActorModel;
        std::string m_AnimationsFile;

        float m_Counter;
        float m_Crono;

        std::string m_CurrentAnimationName;
    };
}
