#include "ModelComponent.hpp"
#include "InfoComponent.hpp"
#include "TransformComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    ModelComponent::ModelComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_RenderPass = table.Get<int>("render_pass", 1);
        m_Model = table.Get<int>("model", "cube");
        m_Material = table.Get<int>("material", "white");
    }

    void ModelSystem::Init(const ActorId &id)
    {
        ModelComponent *model = static_cast<ModelComponent*>(Get(id));
        if (model && !model->Error())
        {
            InfoComponent *info = static_cast<InfoComponent*>(GetComponent(id, "info"));
            if (info)
            {
                Trigger("create_actor_model", Message::CreateActor(id, info->GetEntity(), info->GetVariant()));
            }

            TransformComponent *transform = static_cast<TransformComponent*>(GetComponent(id, "transform"));
            if (transform)
            {
                Trigger("move_actor", Message::MoveActor(id, Position(transform->GetPosition()), RotationMat(transform->GetRotationMatrix())));
            }

            model->Enable();
        }
    }
}
