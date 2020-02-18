#pragma once
#include "../../Framework/Module/Module.hpp"

namespace Tarbora {
    class ActorModel;

    class AnimationController
    {
    public:
        AnimationController(ActorModel *actor, const std::string &file);

        void update(float delta_time);

        void setAnimation(const std::string &name);

    private:
        void updateAnimation();

        ActorModel *actor_model_;
        std::string animations_file_;

        float duration_;
        bool loop_;
        float blend_time_;
        float crono_;
        float counter_;

        std::string current_animation_name_;
    };
}
