#pragma once
#include "../../Framework/Module.hpp"

namespace Tarbora {
    class World;
    class ActorFactory;
    class ActorTest;
    class ActorComponent;
    typedef std::shared_ptr<ActorTest> ActorPtr;
    typedef std::shared_ptr<ActorComponent> ActorComponentPtr;
    typedef std::function<ActorComponentPtr(World *)> ActorComponentCreator;

    class World : public Module
    {
        public:
            World(ActorId maxNumber);
            ~World();

            ActorId Create(std::string entity, glm::vec3 initialPos, glm::vec3 initalRot);
            virtual void Update(float elapsedTime);
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
