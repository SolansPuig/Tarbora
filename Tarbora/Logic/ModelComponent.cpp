#include "ModelComponent.hpp"
#include "InfoComponent.hpp"
#include "TransformComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    ModelComponent::ModelComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        render_pass_ = table.get<int>("render_pass", 1);
        model_ = table.get<int>("model", "cube");
        material_ = table.get<int>("material", "white");
    }

    void ModelSystem::init(const ActorId &id)
    {
        ModelComponent *model = static_cast<ModelComponent*>(get(id));
        if (model && !model->error())
        {
            InfoComponent *info = static_cast<InfoComponent*>(getComponent(id, "info"));
            if (info)
            {
                trigger("create_actor_model", Message::CreateActor(id, info->getEntity(), info->getVariant()));
            }

            TransformComponent *transform = static_cast<TransformComponent*>(getComponent(id, "transform"));
            if (transform)
            {
                trigger("move_actor", Message::MoveActor(id, transform->getPosition(), transform->getRotation()));
            }

            model->enable();
        }
    }
}
