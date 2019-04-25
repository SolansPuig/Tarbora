#pragma once
#include "ActorFactory.hpp"

namespace Tarbora {
    class Actors
    {
        public:
            Actors();
            ~Actors();

            void Init(ActorId maxNumber);
            ActorId Create(std::string entity, glm::vec3 initialPos, glm::vec3 initalRot);
            void Update(float deltaTime);
            void Destroy(ActorId id);
            void Close();
            ActorPtr Get(ActorId id);
            ActorComponentPtr GetComponent(ActorId id, ComponentId compId);

            void AddComponentCreator(std::string name, ActorComponentCreator func);

        private:
            std::vector<ActorTest> m_ActorList;
            ActorFactory m_ActorFactory;
            ActorTest *m_FirstAvailable;
    };
}
