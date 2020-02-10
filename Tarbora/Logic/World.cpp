#include "World.hpp"
#include "InfoComponent.hpp"
#include "TransformComponent.hpp"
#include "TypeComponent.hpp"
#include "ModelComponent.hpp"
#include "PhysicsComponent.hpp"
#include "ControllerComponent.hpp"
#include "../Messages/BasicMessages.hpp"

#define REGISTER_SYSTEM(T) \
    m_Systems.emplace(T::GetName(), std::shared_ptr<T>(new T(this)));\
    m_SystemsOrdered.push_back(T::GetName())

namespace Tarbora {
    World::World() :
        Module(1), m_NextId(0)
    {
        m_MaxFrameRate = 60;

        REGISTER_SYSTEM(InfoSystem);
        REGISTER_SYSTEM(TypeSystem);
        REGISTER_SYSTEM(ModelSystem);
        REGISTER_SYSTEM(ControllerSystem);
        REGISTER_SYSTEM(PhysicsSystem);
        REGISTER_SYSTEM(TransformSystem);

        GetMessageManager()->Subscribe("create_actor", [this](MessageSubject subject, MessageBody *body)
        {
            CreateActorBody m = body->GetContent<CreateActorBody>();
            CreateActor(m.id(), m.entity(), Vec3toGLM(m.position()), Vec3toGLM(m.rotation()));
        });
    }

    ActorId World::CreateActor(ActorId id, const std::string &entity, const glm::vec3 &position, const glm::vec3 &rotation)
    {
        LOG_DEBUG("Creating entity %s", entity.c_str());
        ResourcePtr<LuaScript> resource("entities/" + entity);
        if (resource != nullptr)
        {
            // If it doesn't have an id, set is as a unique number
            if (id == "")
                id = std::to_string(m_NextId++);

            // Everything has an info component
            LuaTable info = resource->CreateTable("info")
                .Set<std::string>("entity", entity)
                .Set<std::string>("variant", "");
            AddComponent(id, "info", info);

            // Everything has a transform component
            LuaTable transform = resource->CreateTable("transform")
                .Set<glm::vec3>("position", position)
                .Set<glm::vec3>("rotation", rotation);
            AddComponent(id, "transform", transform);

            // Create the components
            LuaTable components = resource->Get("components", true);
            for (auto component : components)
            {
                std::string name = component.first.GetAs<std::string>();
                LuaTable value = component.second.GetAs<LuaTable>();

                AddComponent(id, name, value);
            }

            // Initialize all the components
            for (auto sys = m_SystemsOrdered.rbegin(); sys != m_SystemsOrdered.rend(); ++sys)
            {
                m_Systems[*sys]->Init(id);
            }
        }

        return id;
    }

    void World::Update(float deltaTime)
    {
        for (auto sys : m_SystemsOrdered)
        {
            m_Systems[sys]->Update(deltaTime);
        }
    }

    void World::AddComponent(const ActorId &id, const ComponentId &name, const LuaTable &t)
    {
        auto itr = m_Systems.find(name);
        if (itr != m_Systems.end())
        {
            m_Systems[name]->Add(id, t);
        }
        else
        {
            LOG_WARN("World: Couldn't find system named %s.", name.c_str());
        }
    }

    Component *World::GetComponent(const ActorId &id, const ComponentId &compId)
    {
        return m_Systems[compId]->Get(id);
    }

    void World::EnableActor(const ActorId &id)
    {
        for (auto sys : m_Systems)
        {
            sys.second->Enable(id);
        }
    }

    void World::DisableActor(const ActorId &id)
    {
        for (auto sys : m_Systems)
        {
            sys.second->Disable(id);
        }
    }
}
