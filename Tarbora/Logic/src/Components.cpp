#include "../inc/Components.hpp"
#include "../inc/TransformComponent.hpp"
#include "../inc/World.hpp"

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
        m_Texture = "white";
        m_Shader = "model";

        resource->Get(data, "renderPass", &m_RenderPass, {true});
        resource->Get(data, "model", &m_Model, {true});
        resource->Get(data, "texture", &m_Texture, {true, true});
        resource->Get(data, "shader", &m_Shader, {true});

        return true;
    }

    void ModelComponent::AfterInit()
    {
        ActorId id = m_Owner->GetId();
        CreateActorModel(m_World, id, m_Owner->GetEntityName(), "");

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            MoveActor(m_World, id, transform->GetPosition(), transform->GetRotation());
        }
    }
}
