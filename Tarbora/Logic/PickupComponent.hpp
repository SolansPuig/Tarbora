#pragma once
#include "Component.hpp"

namespace Tarbora {
    class ControllerComponent;
    class TransformComponent;

    class PickupComponent : public Component
    {
        friend class PickupSystem;
    public:
        PickupComponent(System *s, const ActorId &id, const LuaTable &table);

    private:
        void pick(const ActorId &id, float distance, const glm::vec3 &position);
        void unpick();
       
        ActorId target_;
        float distance_;

        ControllerComponent *controller_;
        TransformComponent *transform_;
    };

    class PickupSystem : public SystemImpl<PickupComponent>
    {
    public:
        PickupSystem(World *w);

        static std::string getName() { return "pickup"; }

        virtual void init(const ActorId &id);

        virtual void update(float delta_time);
    };
}
