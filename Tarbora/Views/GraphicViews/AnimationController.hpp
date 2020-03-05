#pragma once
#include "../../Framework/Module/Module.hpp"

namespace Tarbora {
    struct Animation
    {
        std::string name;
        float timer;
    };

    class ActorModel;

    class AnimationController
    {
    public:
        AnimationController(ActorModel *actor, const std::string &file);

        void update(float delta_time);

        void setAnimation(const std::string &name);

    private:
        void updateAnimation(float delta_time);

        Animation animation_;

        ActorModel *actor_model_;
        std::string animations_file_;
    };
}
