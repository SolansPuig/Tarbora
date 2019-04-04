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
        bool Create(ActorPtr actor, std::string actorResource);

    protected:
        ActorComponentPtr CreateComponent(json data);
        ActorComponentCreatorMap m_ActorComponentCreators;
    };
}
