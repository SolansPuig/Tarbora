#include "Components.hpp"
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
        CreateActorModelEvent ev = CreateActorModelEvent(m_Owner->GetId(), m_RenderPass, m_Model, m_Shader, m_Texture);
        EventManager::Trigger(CreateActorModel, &ev);
    }
}
