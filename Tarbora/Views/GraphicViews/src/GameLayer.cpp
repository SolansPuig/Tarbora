#include "../inc/GameLayer.hpp"
#include "../../../Messages/BasicMessages.hpp"

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

        m_Scene->CreateSkybox("sky.mat.lua");

        Subscribe("create_actor_model", [this](MessageSubject subject, MessageBody * body)
        {
            LOG_DEBUG("Create actor model");
            CreateActorBody m = body->GetContent<CreateActorBody>();
            if (m_Scene->GetChild(m.id()) != nullptr)
            {
                m_Scene->RemoveChild(m.id());
            }
            m_Scene->CreateActorModel(m.id(), m.entity(), m.variant());
        });

        Subscribe("set_camera", [this](MessageSubject subject, MessageBody *body)
        {
            SetCameraBody m = body->GetContent<SetCameraBody>();
            m_Scene->SetCamera(m.id(), m.name());
        });

        Subscribe("move_actor", [this](MessageSubject subject, MessageBody *body)
        {
            MoveActorBody m = body->GetContent<MoveActorBody>();
            std::shared_ptr<SceneNode> actor = m_Scene->GetChild(m.id());
            actor->SetPosition(Vec3toGLM(m.position()));
            actor->SetRotationMatrix(Mat3toGLM(m.rotation()));
        });

        Subscribe("move_node", [this](MessageSubject subject, MessageBody *body)
        {
            MoveNodeBody m = body->GetContent<MoveNodeBody>();
            std::shared_ptr<SceneNode> node = m_Scene->GetChild(m.id())->GetChild(m.node());
            // node->Set("position", Vec3toGLM(m.position()));
            node->Set("rotation", Vec3toGLM(m.rotation()));
        });

        Subscribe("delete_actor", [this](MessageSubject subject, MessageBody *body)
        {
            DeleteActorBody m = body->GetContent<DeleteActorBody>();
            m_Scene->RemoveChild(m.id());
        });

        Subscribe("set_actor_animation", [this](MessageSubject subject, MessageBody *body)
        {
            SetActorAnimationBody m = body->GetContent<SetActorAnimationBody>();
            m_Scene->AnimateActor(m.id(), m.animation());
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
            Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(0.f,5.f,-5.f)));
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
            m_LookDirection = sensibility * GetInputManager()->GetMouseDelta();

        if (m_Movement != lastMovement)
            Send(1, "set_movement", ApplyPhysics(m_TargetId, 1, glm::normalize(m_Movement)));

        if (m_LookDirection != lastLookDirection)
            Send(1, "look_direction", LookDirection(m_TargetId, m_LookDirection));

        if (m_Jump)
        {
            Send(1, "apply_force", ApplyPhysics(m_TargetId, 1, glm::vec3(0.f, 1.f, 0.f)));
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
