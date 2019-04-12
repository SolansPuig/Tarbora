#include "Components.hpp"
#include "TransformComponent.hpp"
#include "Events.hpp"

namespace Tarbora {
    bool TypeComponent::Init(json data)
    {
        for (auto itr = data["types"].begin(); itr != data["types"].end(); itr++)
            m_Types.push_back(*itr);
        return true;
    }

    bool ModelComponent::Init(json data)
    {
        m_RenderPass = data["renderPass"];
        m_Model = data["model"];
        m_Texture = data["texture"];
        m_Shader = data["shader"];
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
