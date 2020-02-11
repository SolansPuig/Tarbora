#include "GameLayer.hpp"
#include "../../Messages/BasicMessages.hpp"

#define GAMEVIEW(MODULE) static_cast<GraphicView*>(MODULE)

namespace Tarbora {
    GameLayerImpl::GameLayerImpl(GraphicView *view, bool start_active) :
        Layer(view, start_active)
    {
        m_Scene = std::unique_ptr<Scene>(new Scene(GAMEVIEW(m_Module)));

        m_Movement = glm::vec3(0.0f, 0.0f, 0.0f);
        m_LookDirection = glm::vec2(0.0f, 0.0f);
        m_Jump = false;
        m_FreezeMouse = false;

        std::shared_ptr<Camera> camera = m_Scene->CreateCamera("main_camera");
        camera->Set("rotation", glm::vec3(-1.0f, 220.0f, 0.0f));
        camera->Set("position", glm::vec3(-3.0f, -1.5f, -4.0f));
        m_Scene->SetCamera(camera);

        m_Scene->CreateSkybox("sky.mat.lua");

        Subscribe("create_actor_model", [this](MessageSubject subject, MessageBody * body)
        {
            Message::CreateActor m(body);
            if (m_Scene->GetActor(m.GetId()) != nullptr)
            {
                m_Scene->RemoveActor(m.GetId());
            }
            m_Scene->CreateActorModel(m.GetId(), m.GetEntity(), m.GetVariant());
        });

        Subscribe("set_camera", [this](MessageSubject subject, MessageBody *body)
        {
            Message::Node m(body);
            m_Scene->SetCamera(m.GetId(), m.GetName());
        });

        Subscribe("move_actor", [this](MessageSubject subject, MessageBody *body)
        {
            Message::MoveActor m(body);
            std::shared_ptr<SceneNode> actor = m_Scene->GetActor(m.GetId());
            actor->SetPosition(m.GetPosition());
            actor->SetRotationMatrix(m.GetRotation());
        });

        Subscribe("move_node", [this](MessageSubject subject, MessageBody *body)
        {
            Message::MoveNode m(body);
            std::shared_ptr<SceneNode> node = m_Scene->GetActor(m.GetId())->GetChild(m.GetName());
            if (m.HasPosition())
                node->Set("position", m.GetPosition());
            if (m.HasRotation())
                node->Set("rotation", m.GetRotation());
        });

        Subscribe("delete_actor", [this](MessageSubject subject, MessageBody *body)
        {
            Message::Actor m(body);
            m_Scene->RemoveActor(m.GetId());
        });

        Subscribe("set_animation", [this](MessageSubject subject, MessageBody *body)
        {
            Message::SetAnimation m(body);
            m_Scene->AnimateActor(m.GetId(), m.GetAnimation(), m.GetFile());
        });

        m_Scene->SetCamera(m_TargetId, "1st_person");
    }

    bool GameLayerImpl::OnMessage(MessageBody *e)
    {
        return false;
    }

    void GameLayerImpl::GetInput()
    {
        glm::vec3 lastMovement = m_Movement;

        if (GetInputManager()->GetKeyDown(KEY_W))
            m_Movement.z += 1;
        if (GetInputManager()->GetKeyUp(KEY_W))
            m_Movement.z -= 1;
        if (GetInputManager()->GetKeyDown(KEY_S))
            m_Movement.z -= 1;
        if (GetInputManager()->GetKeyUp(KEY_S))
            m_Movement.z += 1;
        if (GetInputManager()->GetKeyDown(KEY_A))
            m_Movement.x += 1;
        if (GetInputManager()->GetKeyUp(KEY_A))
            m_Movement.x -= 1;
        if (GetInputManager()->GetKeyDown(KEY_D))
            m_Movement.x -= 1;
        if (GetInputManager()->GetKeyUp(KEY_D))
            m_Movement.x += 1;
        if (GetInputManager()->GetKeyDown(KEY_SPACE))
            if (!m_Jump) m_Jump = true;

        if (GetInputManager()->GetKeyDown(KEY_X))
        {
            Send(1, "create_actor", Message::CreateActor("", "cube.lua", "", Position(glm::vec3(0.f,1.f,-5.f))));
        }
        if (GetInputManager()->GetKeyDown(KEY_Y))
        {
            static bool thirdPerson = true;
            thirdPerson = !thirdPerson;
            std::string cameraMode = (thirdPerson ? "3rd_person" : "1st_person");

            LOG_DEBUG("Set camera %s", cameraMode.c_str());
            m_Scene->SetCamera(m_TargetId, cameraMode);
        }

        glm::vec2 lastLookDirection = m_LookDirection;
        float sensibility = 0.04; // TODO: Change this to a config file

        if (!m_FreezeMouse)
        {
            m_LookDirection = sensibility * GetInputManager()->GetMouseDelta();
        }
        if (m_Movement != lastMovement)
        {
            Send(1, "set_movement", Message::ApplyPhysics(m_TargetId, 1, Direction(glm::normalize(m_Movement))));
        }
        if (m_LookDirection != lastLookDirection)
        {
            Send(1, "look_direction", Message::LookAt(m_TargetId, Direction(glm::vec3(m_LookDirection, 0.f))));
        }
        if (m_Jump)
        {
            Send(1, "apply_force", Message::ApplyPhysics(m_TargetId, 1, Direction(glm::vec3(0.f, 1.f, 0.f))));
            m_Jump = false;
        }
    }

    void GameLayerImpl::Update(float deltaTime)
    {
        m_Scene->Update(deltaTime);
    }

    void GameLayerImpl::Draw()
    {
        m_Scene->Draw();
    }
}
