#pragma once
#include "ActorFactory.hpp"
#include "../../Framework/ClientApplication.hpp"

namespace Tarbora {
    class Actors : public ClientApplication
    {
        public:
            Actors(std::string server_address, ActorId maxNumber);
            ~Actors();

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
