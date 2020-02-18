#pragma once
#include "../Framework/Framework.hpp"

namespace Tarbora {
    class System;
    class Component;

    class World : public Module
    {
        public:
            World();

            ActorId createActor(ActorId id, const std::string &entity, const glm::vec3 &p, const glm::quat &r);

            void update(float delta_time);

            void addComponent(const ActorId &id, const ComponentId &component, const LuaTable &table);

            Component* getComponent(const ActorId &id, const ComponentId &component);

            void enableActor(const ActorId &id);

            void disableActor(const ActorId &id);


        private:
            std::unordered_map<ComponentId, std::shared_ptr<System>> systems_;
            std::vector<ComponentId> systems_order_;
            long next_id_;
    };
}
