#pragma once
#include "../Framework/Module.hpp"

namespace Tarbora {
    class System;
    class Component;

    class World : public Module
    {
        public:
            World();

            ActorId CreateActor(ActorId id, const std::string &entity, const glm::vec3 &position, const glm::vec3 &rotation);

            void Update(float deltaTime);

            void AddComponent(const ActorId &id, const ComponentId &name, const LuaTable &t);

            Component *GetComponent(const ActorId &id, const ComponentId &compId);

            void EnableActor(const ActorId &id);

            void DisableActor(const ActorId &id);


        private:
            std::unordered_map<ComponentId, std::shared_ptr<System>> m_Systems;
            std::vector<ComponentId> m_SystemsOrdered;
            long m_NextId;
    };
}
