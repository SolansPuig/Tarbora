#pragma once
#include "ActorFactory.hpp"
#include "../../Framework/Module.hpp"

namespace Tarbora {
    class World : public Module
    {
        public:
            World(std::string server_address, ActorId maxNumber);
            ~World();

            ActorId Create(std::string entity, glm::vec3 initialPos, glm::vec3 initalRot);
            void Update(float deltaTime);
            void Destroy(ActorId id);

            ActorPtr Get(ActorId id);
            ActorComponentPtr GetComponent(ActorId id, ComponentId compId);

            void AddComponentCreator(std::string name, ActorComponentCreator func);

        private:
            std::vector<ActorTest> m_ActorList;
            std::unique_ptr<ActorFactory> m_ActorFactory;
            ActorTest *m_FirstAvailable;
    };
}
