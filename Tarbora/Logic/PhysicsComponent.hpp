#pragma once
#include "Component.hpp"
#include "PhysicsEngine/RigidBody.hpp"

namespace Tarbora {
    class TransformComponent;

    class PhysicsComponent : public Component
    {
        friend class PhysicsSystem;
    public:
        PhysicsComponent(System *s, const ActorId &id, const LuaTable &table);

        void setTransform(const glm::vec3 &position, const glm::quat &rotation);
       
        void setAngularFactor(const glm::vec3 &direction);

        void setLinearDamping(float value);

        void setVelocity(const glm::vec3 &direction);

        void setAngularVelocity(const glm::vec3 &direction);

        void applyForce(const glm::vec3 &direction);

        void applyTorque(const glm::vec3 &direction);

        void stop();

        std::shared_ptr<RayCastResult> rayCast(const glm::vec3 &origin, const glm::quat &direction, float length);

        float getHeight() { return height_; }

        void pick();
        
        void pick(const glm::vec3 &position);

        void movePicked(const glm::vec3 &position);

        void unpick();



    private:
        virtual void onEnable();
        virtual void onDisable();

        std::shared_ptr<RigidBody> body_;
        TransformComponent *transform_;
        ActorMotionState *motion_state_;
        float height_;
    };

    class PhysicsSystem : public SystemImpl<PhysicsComponent>
    {
    public:
        PhysicsSystem(World *w);
        ~PhysicsSystem();

        static std::string getName() { return "physics"; }

        virtual void init(const ActorId &id);

        virtual void update(float delta_time);
    };
}
