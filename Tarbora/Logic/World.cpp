#include "World.hpp"
#include "InfoComponent.hpp"
#include "TransformComponent.hpp"
#include "TypeComponent.hpp"
#include "ModelComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ControllerComponent.hpp"
#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

#define REGISTER_SYSTEM(T) \
    systems_.emplace(T::getName(), std::shared_ptr<T>(new T(this)));\
    systems_order_.push_back(T::getName())

namespace Tarbora {
    World::World() :
        Module(1)
    {
        max_fps_ = 60;
        next_id_ = 0;

        REGISTER_SYSTEM(InfoSystem);
        REGISTER_SYSTEM(TypeSystem);
        REGISTER_SYSTEM(ControllerSystem);
        REGISTER_SYSTEM(AnimationSystem);
        REGISTER_SYSTEM(ModelSystem);
        REGISTER_SYSTEM(PhysicsSystem);
        REGISTER_SYSTEM(TransformSystem);

        getMessageManager()->subscribe("create_actor", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::CreateActor m(body);
            createActor(m.getId(), m.getEntity(), m.getPosition(), m.getRotation());
        });
    }

    ActorId World::createActor(ActorId id, const std::string &entity, const glm::vec3 &position, const glm::quat &rotation)
    {
        ResourcePtr<LuaScript> resource("entities/" + entity);
        if (resource != nullptr)
        {
            // If it doesn't have an id, set is as a unique number
            if (id == "")
                id = std::to_string(next_id_++);

            // Everything has an info component
            LuaTable info = resource->createTable("info");
            info.set<std::string>("entity", entity);
            info.set<std::string>("variant", "");
            addComponent(id, "info", info);

            // Everything has a transform component
            LuaTable transform = resource->createTable("transform");
            transform.set<glm::vec3>("position", position);
            transform.set<glm::vec3>("rotation", glm::degrees(glm::eulerAngles(rotation)));
            addComponent(id, "transform", transform);

            // Create the components
            LuaTable components = resource->get("components", true);
            for (auto component : components)
            {
                std::string name = component.first.getAs<std::string>();
                LuaTable value = component.second.getAs<LuaTable>();

                addComponent(id, name, value);
            }

            // Initialize all the components
            for (auto sys = systems_order_.rbegin(); sys != systems_order_.rend(); ++sys)
            {
                systems_[*sys]->init(id);
            }
        }

        return id;
    }

    void World::update(float delta_time)
    {
        for (auto sys : systems_order_)
        {
            systems_[sys]->update(delta_time);
        }
    }

    void World::addComponent(const ActorId &id, const ComponentId &component, const LuaTable &t)
    {
        auto itr = systems_.find(component);
        if (itr != systems_.end())
        {
            itr->second->add(id, t);
        }
        else
        {
            LOG_WARN("World: Couldn't find system named %s.", component.c_str());
        }
    }

    Component *World::getComponent(const ActorId &id, const ComponentId &component)
    {
        auto itr = systems_.find(component);
        if (itr != systems_.end())
        {
            return itr->second->get(id);
        }
        return nullptr;
    }

    void World::enableActor(const ActorId &id)
    {
        for (auto sys : systems_)
        {
            sys.second->enable(id);
        }
    }

    void World::disableActor(const ActorId &id)
    {
        for (auto sys : systems_)
        {
            sys.second->disable(id);
        }
    }
}
