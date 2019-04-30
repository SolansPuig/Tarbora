#pragma once
#include <functional>
#include "Actor.hpp"

namespace Tarbora {
    typedef std::function<ActorComponentPtr(void)> ActorComponentCreator;

    class ActorFactory {
        typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;
    public:
        ActorFactory();
        void AddComponentCreator(std::string name, ActorComponentCreator func);
        bool Create(ActorTest *actor, std::string actorResource, glm::vec3 initialPos, glm::vec3 initalRot);

    protected:
        ActorComponentPtr CreateComponent(JsonPtr resource, std::string name, raw_json data);
        ActorComponentCreatorMap m_ActorComponentCreators;
    };
}
