#include "ActorFactory.hpp"
#include "Logger.hpp"

namespace Tarbora {
    ActorFactory::ActorFactory() {}

    bool ActorFactory::Create(ActorPtr actor, std::string actorResource)
    {
        json j = GET_RESOURCE(JsonResource, actorResource)->GetJson();
        if (!actor->Init(j))
        {
            LOG_ERR("ActorFactory: Failed to initialize actor %s.", actorResource.c_str());
            return false;
        }
        json components = j["components"];
        for (auto itr = components.begin(); itr != components.end(); itr++)
        {
            ActorComponentPtr component = ActorComponentPtr(CreateComponent(*itr));
            if (component)
            {
                actor->AddComponent(component);
                component->SetOwner(actor);
            } else
            {
                return false;
            }
        }
        actor->AfterInit();
        return true;
    }

    void ActorFactory::AddComponentCreator(std::string name, ActorComponentCreator func)
    {
        m_ActorComponentCreators[name] = func;
    }

    ActorComponentPtr ActorFactory::CreateComponent(json data)
    {
        std::string name = data["name"];

        ActorComponentPtr component;
        auto itr = m_ActorComponentCreators.find(name);
        if (itr != m_ActorComponentCreators.end())
        {
            ActorComponentCreator creator = itr->second;
            component = creator();
        } else
        {
            LOG_ERR("ActorFactory: Couldn't find ActorComponent named %s.", name.c_str());
            return ActorComponentPtr();
        }

        if (component)
        {
            if (!component->Init(data))
            {
                LOG_ERR("ActorFactory: Component %s failed to initialize.", name.c_str());
                return ActorComponentPtr();
            }
        }

        return component;
    }
}
