#include "../inc/Components.hpp"
#include "../inc/TransformComponent.hpp"
#include "../inc/World.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    bool TypeComponent::Init(JsonPtr resource, raw_json data)
    {
        for (unsigned int i = 0; i < data.size(); i++)
        {
            m_Types.push_back(resource->GetString(data, i));
        }

        return true;
    }

    bool ModelComponent::Init(JsonPtr resource, raw_json data)
    {
        m_RenderPass = 1;
        m_Model = "cube";
        m_Material = "white";

        resource->Get(data, "renderPass", &m_RenderPass, {true});
        resource->Get(data, "model", &m_Model, {true});
        resource->Get(data, "material", &m_Material, {true, true});

        return true;
    }

    void ModelComponent::AfterInit()
    {
        ActorId id = m_Owner->GetId();
        Trigger("create_actor_model", CreateActor(id, m_Owner->GetEntityName(), ""));

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            Trigger("move_actor", MoveActor(id, transform->GetPosition(), transform->GetRotation()));
        }
    }
}
