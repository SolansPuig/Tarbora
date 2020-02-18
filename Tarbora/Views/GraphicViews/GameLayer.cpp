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
            Message::CreateActor m(body);
            if (scene_->getActor(m.getId()) != nullptr)
            {
                scene_->removeActor(m.getId());
            }
            scene_->createActorModel(m.getId(), m.getEntity(), m.getVariant());
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
            actor->setPosition(m.getPosition());
            actor->setRotation(m.getRotation());
        });

        subscribe("move_node", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::MoveNode m(body);
            std::shared_ptr<SceneNode> node = scene_->getActor(m.getId())->getChild(m.getName());
            if (m.hasPosition())
                node->setPosition(m.getPosition());
            if (m.hasRotation())
                node->setRotation(m.getRotation());
        });

        subscribe("delete_actor", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::Actor m(body);
            scene_->removeActor(m.getId());
        });

        subscribe("set_animation", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::SetAnimation m(body);
            scene_->animateActor(m.getId(), m.getAnimation(), m.getFile());
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
            send(1, "create_actor", Message::CreateActor("", "cube.lua", "", glm::vec3(0.f, 1.f, -5.f)));
        }
        if (getInputManager()->getKeyDown(KEY_Y))
        {
            static bool third_person = true;
            third_person = !third_person;
            const std::string camera_mode = (third_person ? "3rd_person" : "1st_person");

            LOG_DEBUG("Set camera %s", camera_mode.c_str());
            scene_->setCamera(target_id_, camera_mode);
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
            send(1, "set_angular_velocity", Message::ApplyPhysics(target_id_, glm::vec3(0.f, look_direction_.x, 0.f)));
            send(1, "look_direction", Message::LookAt(target_id_, glm::vec3(0.f, look_direction_.y, 0.f)));
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
