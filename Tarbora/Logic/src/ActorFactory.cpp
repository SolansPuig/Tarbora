#include "../inc/ActorFactory.hpp"
#include "../inc/World.hpp"

namespace Tarbora {
    ActorFactory::ActorFactory(World *world) :
        m_World(world)
    {
    }

    bool ActorFactory::Create(ActorTest *actor, std::string actorResource, glm::vec3 initialPos, glm::vec3 initialRot)
    {
        JsonPtr resource = GET_RESOURCE(Json, "entities/" + actorResource);
        if (resource != NULL)
        {
            if (!actor->Init(actorResource))
            {
                LOG_ERR("ActorFactory: Failed to initialize actor %s.", actorResource.c_str());
                return false;
            }

            // Everyting has a transform component
            raw_json transform;
            transform["position"] = { initialPos.x, initialPos.y, initialPos.z };
            transform["rotation"] = { initialRot.x, initialRot.y, initialRot.z };
            ActorComponentPtr component = ActorComponentPtr(CreateComponent(resource, "transform", transform));
            actor->AddComponent(component);

            raw_json components;
            resource->Get("components", &components, {true, true});
            resource->PushErrName("components");

            for (auto &itr : components.items())
            {
                resource->PushErrName(itr.key());
                ActorComponentPtr component = ActorComponentPtr(CreateComponent(resource, itr.key(), itr.value()));
                resource->PopErrName();
                if (component)
                {
                    actor->AddComponent(component);
                    component->SetOwner(actor);
                }
            }

            resource->PopErrName();
            actor->AfterInit();
            return true;
        }
        else
        {
            return false;
        }
    }

    void ActorFactory::AddComponentCreator(std::string name, ActorComponentCreator func)
    {
        m_ActorComponentCreators[name] = func;
    }

    ActorComponentPtr ActorFactory::CreateComponent(JsonPtr resource, std::string name, raw_json data)
    {
        ActorComponentPtr component;
        auto itr = m_ActorComponentCreators.find(name);
        if (itr != m_ActorComponentCreators.end())
        {
            ActorComponentCreator creator = itr->second;
            component = creator();
            component->m_World = m_World;
        } else
        {
            LOG_WARN("ActorFactory: Couldn't find ActorComponent named %s.", name.c_str());
            return ActorComponentPtr();
        }

        if (component)
        {
            if (!component->Init(resource, data))
            {
                LOG_ERR("ActorFactory: Component %s failed to initialize.", name.c_str());
                return ActorComponentPtr();
            }
        }

        return component;
    }
}
