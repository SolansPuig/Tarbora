#include "../inc/Components.hpp"
#include "../inc/TransformComponent.hpp"

namespace Tarbora {
    bool TypeComponent::Init(JsonPtr resource, json data)
    {
        for (unsigned int i = 0; i < data.size(); i++)
        {
            m_Types.push_back(resource->GetString(data, i));
        }

        return true;
    }

    bool ModelComponent::Init(JsonPtr resource, json data)
    {
        m_RenderPass = 1;
        m_Model = "models/cube.json";
        m_Texture = "textures/white.png";
        m_Shader = "shaders/model.shader.json";

        resource->Get(data, "renderPass", &m_RenderPass, {true});
        resource->Get(data, "model", &m_Model, {true});
        resource->Get(data, "texture", &m_Texture, {true, true});
        resource->Get(data, "shader", &m_Shader, {true});

        return true;
    }

    void ModelComponent::AfterInit()
    {
        ActorId id = m_Owner->GetId();
        CreateActorModelEvent ev = CreateActorModelEvent(id, m_RenderPass, m_Model, m_Shader, m_Texture);
        EventManager::Trigger(CreateActorModel, &ev);

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            ActorMoveEvent ev(id, transform->GetPosition(), transform->GetRotation());
            EventManager::Trigger(EventType::ActorMove, &ev);
        }
    }
}
