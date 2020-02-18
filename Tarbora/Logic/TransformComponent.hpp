#pragma once
#include "Component.hpp"

namespace Tarbora {
    class PhysicsComponent;

    class TransformComponent : public Component
    {
        friend class TransformSystem;
    public:
        TransformComponent(System *s, const ActorId &id, const LuaTable &table);

        void setController(PhysicsComponent *controller) { controller_ = controller; }
        void setPosition(const glm::vec3 &position);
        void setRotation(const glm::vec3 &rotation);
        void setRotation(const glm::quat &rotation);
        void setOrigin(const glm::vec3 &origin);
        void move(const glm::vec3 &position);
        void rotate(const glm::vec3 &rotation);
        void rotate(const glm::quat &rotation);

        const glm::vec3& getPosition() { return position_; }
        const glm::quat& getRotation() { return rotation_; }
        glm::vec3 getEulerRotation() { return glm::eulerAngles(rotation_); }
        const glm::vec3& getOrigin() { return origin_; }

    private:
        PhysicsComponent *controller_;

        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 origin_;
        bool changed_;
    };

    class TransformSystem : public SystemImpl<TransformComponent>
    {
    public:
        TransformSystem(World *w);

        static std::string getName() { return "transform"; }
        virtual void update(float delta_time);
    };
}
