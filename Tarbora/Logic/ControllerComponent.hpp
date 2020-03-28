#pragma once
#include "Component.hpp"

namespace Tarbora {
    class TransformComponent;
    class PhysicsComponent;
    class AnimationComponent;

    struct LookAt
    {
        ActorId target;
        float distance;
        glm::quat direction;
        glm::vec3 position;
    };

    class ControllerComponent : public Component
    {
        friend class ControllerSystem;
    public:
        ControllerComponent(System *s, const ActorId &id, const LuaTable &table);

        void setMovement(const glm::vec3 &direction);

        void setLookDirection(const glm::vec3 &direction);

        const glm::quat& getLookDirection() { return look_direction_; }

        LookAt getLookAt() { return LookAt{target_, target_distance_, look_direction_, target_position_}; }

    private:
        void checkGround();
        void checkLookAt();

        TransformComponent *transform_;
        PhysicsComponent *physics_;
        AnimationComponent *animation_;
        float speed_;
        float run_speed_;
        glm::vec3 movement_{glm::vec3(0.f)};
        glm::vec3 facing_{glm::vec3(0.f)};
        glm::quat look_direction_{glm::quat()};
        glm::vec3 target_position_{glm::vec3(0.f)};
        bool on_ground_;

        ActorId target_;
        float target_distance_;
    };

    class ControllerSystem : public SystemImpl<ControllerComponent>
    {
    public:
        ControllerSystem(World *w);

        static std::string getName() { return "controller"; }

        virtual void init(const ActorId &id);

        virtual void update(float delta_time);
    };
}
