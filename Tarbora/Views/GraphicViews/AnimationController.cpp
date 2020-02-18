#include "AnimationController.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
    AnimationController::AnimationController(ActorModel *actor, const std::string &file) :
        actor_model_(actor), animations_file_("animations/" + file)
    {
        crono_ = 0.f;
        counter_ = 0.f;
    }

    void AnimationController::update(float delta_time)
    {
        crono_ += delta_time;
        counter_ += delta_time;
        if (blend_time_ > 0.f)
            blend_time_ -= delta_time;
        if (((loop_ || crono_ <= duration_)))
        {
            updateAnimation();
            counter_ = 0.f;
        }
    }

    void AnimationController::setAnimation(const std::string &name)
    {
        current_animation_name_ = name;
        crono_ = 0.0f;

        ResourcePtr<LuaScript> resource(animations_file_);
        LuaTable animation = resource->get(current_animation_name_, true);
        loop_ = animation.get<bool>("loop");
        duration_ = animation.get<float>("duration");
        blend_time_ = animation.get<float>("blend_time");
    }

    void AnimationController::updateAnimation()
    {
        ResourcePtr<LuaScript> resource(animations_file_);
        LuaTable animation = resource->get(current_animation_name_, true);
        LuaTable query = resource->get("query");
        query.set("time", crono_);

        if (animation.valid() && query.valid())
        {
            LuaTable current_animation = animation.get("nodes");

            for (auto itr : current_animation)
            {
                std::string node_name = itr.first.getAs<std::string>();
                LuaTable node_lua = itr.second.getAs<LuaTable>();
                std::shared_ptr<SceneNode> node = actor_model_->nodes_[node_name];

                for (auto property : node_lua)
                {
                    std::string name = property.first.getAs<std::string>();
                    glm::vec3 value = property.second.getAs<glm::vec3>();

                    if (name == "position")
                    {
                        glm::vec3 current_value = node->getPosition();
                        query.set("currentX", current_value.x * 100.f);
                        query.set("currentY", current_value.y * 100.f);
                        query.set("currentZ", current_value.z * 100.f);
                        node->setPosition(value/100.f);
                    }
                    else if (name == "rotation")
                    {
                        glm::vec3 current_value = node->getRotation();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        query.set("currentZ", current_value.z);
                        node->setRotation(value);
                    }
                    else if (name == "scale")
                    {
                        glm::vec3 current_value = node->getScale();
                        query.set("currentX", current_value.x * 100.f);
                        query.set("currentY", current_value.y * 100.f);
                        query.set("currentZ", current_value.z * 100.f);
                        node->setScale(value/100.f);
                    }
                    else if (name == "uv_map" && node->getNodeType() == "MESH")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        glm::tvec2<unsigned short> current_value = mesh->getUvMap();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        mesh->setUvMap(value);
                    }
                    else if (name == "color_primary" && node->getNodeType() == "MESH")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        glm::vec3 current_value = mesh->getColorPrimary();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        query.set("currentZ", current_value.z);
                        mesh->setColorPrimary(value);
                    }
                    else if (name == "color_secondary" && node->getNodeType() == "MESH")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        glm::vec3 current_value = mesh->getColorSecondary();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        query.set("currentZ", current_value.z);
                        mesh->setColorSecondary(value);
                    }
                    else if (name == "color_detail1" && node->getNodeType() == "MESH")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        glm::vec3 current_value = mesh->getColorDetail();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        query.set("currentZ", current_value.z);
                        mesh->setColorDetail(value);
                    }
                    else if (name == "color_detail2" && node->getNodeType() == "MESH")
                    {
                        auto mesh = std::static_pointer_cast<MeshNode>(node);
                        glm::vec3 current_value = mesh->getColorDetail2();
                        query.set("currentX", current_value.x);
                        query.set("currentY", current_value.y);
                        query.set("currentZ", current_value.z);
                        mesh->setColorDetail2(value);
                    }
                }
            }
        }
    }
}
