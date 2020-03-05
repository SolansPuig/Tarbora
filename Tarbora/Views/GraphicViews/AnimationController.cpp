#include "AnimationController.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
    AnimationController::AnimationController(ActorModel *actor, const std::string &file) :
        actor_model_(actor), animations_file_("animations/" + file)
    {
    }

    void AnimationController::update(float delta_time)
    {
        if (animation_.name != "")
        {
            updateAnimation(delta_time);
        }
    }

    void AnimationController::setAnimation(const std::string &name)
    {
        animation_.name = name;
        animation_.timer = 0.0f;
    }

    void AnimationController::updateAnimation(float delta_time)
    {
        ResourcePtr<LuaScript> resource(animations_file_);
        LuaTable animation = resource->get(animation_.name, true);

        if (animation.valid())
        {
            LuaTable nodes = animation.get("nodes");
            float duration = animation.get<float>("duration");
            float step = animation.get<float>("step", 1, true);
            float current_frame = animation_.timer / step;

            for (auto itr : nodes)
            {
                std::string node_name = itr.first.getAs<std::string>();
                LuaTable node_lua = itr.second.getAs<LuaTable>();
                std::shared_ptr<SceneNode> n = actor_model_->nodes_[node_name];
                if (n->getNodeType() == "ANIMATED")
                {
                    auto node = std::static_pointer_cast<AnimatedNode>(n);

                    for (auto property : node_lua)
                    {
                        std::string name = property.first.getAs<std::string>();
                        LuaTable keyframes = property.second.getAs<LuaTable>();

                        int previous_frame = 0;
                        int next_frame = 0;
                        for (auto keyframe : keyframes)
                        {
                            int frame = keyframe.first.getAs<int>();
                            if (frame <= current_frame)
                                previous_frame = frame;
                            else
                            {
                                next_frame = frame;
                                break;
                            }
                        }
                        float distance = (next_frame > 0 ? (next_frame - previous_frame) : (duration - previous_frame));
                        float lerp_factor = (current_frame - previous_frame)/(distance > 0 ? distance : 1);

                        if (name == "position")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setPositionAnimation(glm::lerp(previous_value, next_value, lerp_factor)/100.f);
                        }
                        else if (name == "rotation")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setRotationAnimation(glm::mix(
                                glm::quat(glm::radians(previous_value)),
                                glm::quat(glm::radians(next_value)),
                                lerp_factor));
                        }
                        else if (name == "scale")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            glm::vec3 new_value = glm::lerp(previous_value, next_value, lerp_factor);
                            node->setScaleAnimation(new_value/100.f);
                        }
                        else if (name == "global_scale")
                        {
                            float previous_value = keyframes.get<float>(previous_frame);
                            float next_value = keyframes.get<float>(next_frame);
                            node->setGlobalScaleAnimation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                        else if (name == "uv_map")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setUvMapAnimation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                        else if (name == "color_primary")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setColorPrimaryAnimation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                        else if (name == "color_secondary")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setColorSecondaryAnimation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                        else if (name == "color_detail1")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setColorDetailAnimation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                        else if (name == "color_detail2")
                        {
                            glm::vec3 previous_value = keyframes.get<glm::vec3>(previous_frame);
                            glm::vec3 next_value = keyframes.get<glm::vec3>(next_frame);
                            node->setColorDetail2Animation(glm::lerp(previous_value, next_value, lerp_factor));
                        }
                    }

                    animation_.timer += delta_time;
                    if (current_frame >= duration)
                    {
                        if (animation.get<bool>("loop", true, true))
                            animation_.timer = 0.f;
                        else
                            animation_.name = "";
                    }
                }
            }
        }
    }
}
