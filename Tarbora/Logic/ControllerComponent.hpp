#pragma once
#include "Component.hpp"

namespace Tarbora {
    class TransformComponent;
    class PhysicsComponent;
    class AnimationComponent;

    class ControllerComponent : public Component
    {
        friend class ControllerSystem;
    public:
        ControllerComponent(System *s, const ActorId &id, const LuaTable &table);

        void setMovement(const glm::vec3 &direction);

        void setLookDirection(const glm::vec3 &direction);

    private:
        void checkGround();
        void checkLookAt();

        TransformComponent *transform_;
        PhysicsComponent *physics_;
        AnimationComponent *animation_;
        float speed_;
        float run_speed_;
        glm::vec3 movement_;
        glm::vec3 facing_;
        glm::quat look_direction_;
        bool on_ground_;

        ActorId target_;
        int target_distance_;
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
