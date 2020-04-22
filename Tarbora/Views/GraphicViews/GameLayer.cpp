#include "GameLayer.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    GameLayer::GameLayer(HumanView *view, bool start_active) :
        Layer(view, start_active)
    {
        scene_ = std::shared_ptr<Scene>(new Scene(view));

        movement_ = glm::vec3(0.0f, 0.0f, 0.0f);
        look_direction_ = glm::vec2(0.0f, 0.0f);
        jump_ = false;
        freeze_mouse_ = false;

        std::shared_ptr<Camera> camera = scene_->createCamera("main_camera");
        camera->setRotation(glm::vec3(-1.0f, 180.0f, 0.0f));
        camera->setPosition(glm::vec3(-3.0f, -1.5f, 10.0f));
        scene_->setCamera(camera);

        scene_->createSkybox("sky.mat.lua");

        subscribe("create_actor_model", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::CreateActorModel m(body);
            scene_->createActorModel(m.getId(), (RenderPass)m.getRenderPass(), m.getModel(), m.getMaterial());
        });

        subscribe("set_camera", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::Node m(body);
            scene_->setCamera(m.getId(), m.getName());
        });

        subscribe("move_actor", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::MoveActor m(body);
            std::shared_ptr<SceneNode> actor = scene_->getActor(m.getId());
            if (actor)
            {
                actor->setPosition(m.getPosition());
                actor->setRotation(m.getOrientation());
            }
        });

        subscribe("move_node", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::MoveNode m(body);
            std::shared_ptr<SceneNode> actor = scene_->getActor(m.getId());
            if (actor)
            {
                std::shared_ptr<SceneNode> node = actor->getChild(m.getName());
                if (node)
                {
                    if (m.hasPosition())
                        node->setPosition(m.getPosition());
                    if (m.hasOrientation())
                        node->setRotation(m.getOrientation());
                }
            }
        });

        subscribe("delete_actor", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::Actor m(body);
            scene_->removeActor(m.getId());
        });

        subscribe("start_animation", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::StartAnimation m(body);
            auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
            if (actor)
            {
                actor->startAnimation(Animation{m.getAnimation(), m.getFile(), (BlendMode)m.getBlendMode(), m.getBlendFactor(), m.getFadeInTimer(), m.getLoop()});
            }

        });

        subscribe("start_base_animation", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::StartAnimation m(body);
            auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
            if (actor)
            {
                actor->startAnimation(Animation{m.getAnimation(), m.getFile(), (BlendMode)m.getBlendMode(), m.getBlendFactor(), m.getFadeInTimer(), m.getLoop()}, true);
            }
        });

        subscribe("end_animation", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::EndAnimation m(body);
            auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
            if (actor)
            {
                actor->endAnimation(m.getAnimation(), (StopMode)m.getStopMode(), m.getFadeOutTimer());
            }
        });


        scene_->setCamera(target_id_, "1st_person");
    }

    bool GameLayer::onMessage(const MessageBody &body)
    {
        // TODO
        UNUSED(body);
        return false;
    }

    void GameLayer::getInput()
    {
        glm::vec3 last_movement = movement_;

        if (getInputManager()->getKeyDown(KEY_W))
            movement_.z += 1;
        if (getInputManager()->getKeyUp(KEY_W))
            movement_.z -= 1;
        if (getInputManager()->getKeyDown(KEY_S))
            movement_.z -= 1;
        if (getInputManager()->getKeyUp(KEY_S))
            movement_.z += 1;
        if (getInputManager()->getKeyDown(KEY_A))
            movement_.x += 1;
        if (getInputManager()->getKeyUp(KEY_A))
            movement_.x -= 1;
        if (getInputManager()->getKeyDown(KEY_D))
            movement_.x -= 1;
        if (getInputManager()->getKeyUp(KEY_D))
            movement_.x += 1;
        if (getInputManager()->getKeyDown(KEY_SPACE))
            if (!jump_) jump_ = true;

        if (getInputManager()->getKeyDown(KEY_X))
        {
            Message::CreateActor msg("", "cube.lua");
            msg.setPosition(glm::vec3(0.f, 1.f, -5.f));
            send(1, "create_actor", msg);
        }
        if (getInputManager()->getKeyDown(KEY_Y))
        {
            static bool third_person = true;
            third_person = !third_person;
            const std::string camera_mode = (third_person ? "3rd_person" : "1st_person");

            LOG_DEBUG("Set camera %s", camera_mode.c_str());
            scene_->setCamera(target_id_, camera_mode);
        }
        if (getInputManager()->getKeyDown(KEY_P))
        {
            pick_object_ = !pick_object_;
           
            if (pick_object_)
                send(1, "grab", Message::Actor(target_id_));
            else
                send(1, "release", Message::Actor(target_id_));
        }
        if (pick_object_ && getInputManager()->mouseScrolled())
        {
            send(1, "grab_distance", Message::LookAt(target_id_, getInputManager()->getScrollDelta()));
        }

        glm::vec2 last_look_direction_ = look_direction_;
        float sensibility = 0.04; // TODO: Change this to a config file

        if (!freeze_mouse_)
        {
            look_direction_ = sensibility * getInputManager()->getMouseDelta();
        }
        if (movement_ != last_movement)
        {
            send(1, "set_movement", Message::ApplyPhysics(target_id_, glm::normalize(movement_)));
        }
        if (look_direction_ != last_look_direction_)
        {
            send(1, "set_rotation", Message::ApplyPhysics(
                     target_id_,
                     glm::vec3(0.f, look_direction_.x, 0.f)
                 ));
            send(1, "set_facing", Message::ApplyPhysics(
                     target_id_,
                     glm::vec3(-look_direction_.y, 0.f, 0.f)
                 ));
        }
        if (jump_)
        {
            send(1, "apply_force", Message::ApplyPhysics(target_id_, glm::vec3(0.f, 1.f, 0.f)));
            jump_ = false;
        }
    }

    void GameLayer::update(float delta_time)
    {
        scene_->update(delta_time);
    }

    void GameLayer::draw()
    {
        scene_->draw();
    }
}
