#include "../inc/GameLayer.hpp"
#include "../../../Messages/BasicMessages.hpp"

#define GAMEVIEW(MODULE) static_cast<GraphicView*>(MODULE)

namespace Tarbora {
    GameLayerImpl::GameLayerImpl(GraphicView *view, bool start_active) :
        Layer(view, start_active)
    {
        m_Scene = std::unique_ptr<Scene>(new Scene(GAMEVIEW(m_Module)));

        m_Movement = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Jump = false;

        JsonPtr settings = GET_RESOURCE(Json, "Settings.json");
        if (settings)
        {
            raw_json controls = settings->GetJson("controls");
            settings->PushErrName("controls");

            settings->Get(controls, "jump_impulse", &m_JumpImpulse);
            settings->Get(controls, "direction_impulse", &m_DirectionImpulse);
            settings->Get(controls, "torque_impulse", &m_TorqueImpulse);

            settings->PopErrName();
        }

        m_Scene->CreateSkybox("sky.shader.json", "sky.png");

        Subscribe("create_actor_model", [this](MessageSubject subject, MessageBody * body)
        {
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
    }

    bool GameLayerImpl::OnMessage(MessageBody *e)
    {
        return false;
    }

    void GameLayerImpl::GetInput()
    {
        static glm::vec3 movement(0.f, 0.f, 0.f);
        static bool jump = false;

        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_W))
        {
            movement.z += 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyUp(KEY_W))
        {
            movement.z -= 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_S))
        {
            movement.z -= 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyUp(KEY_S))
        {
            movement.z += 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_A))
        {
            movement.x += 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyUp(KEY_A))
        {
            movement.x -= 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_D))
        {
            movement.x -= 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyUp(KEY_D))
        {
            movement.x += 1;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_SPACE))
        {
            if (!jump) jump = true;
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_X))
        {
            Send(1, "create_actor", CreateActor(0, "cube.json", "", glm::vec3(0.f,5.f,-5.f)));
        }
        if (GAMEVIEW(m_Module)->Input()->GetKeyDown(KEY_Y))
        {
            static bool thirdPerson = false;
            LOG_DEBUG("Third person %d", thirdPerson);
            if (thirdPerson) m_Scene->SetCamera(5, "1st_person");
            else m_Scene->SetCamera(5, "3rd_person");

            thirdPerson = !thirdPerson;
        }


        Send(1, "set_velocity", ApplyPhysics(m_TargetId, m_DirectionImpulse, glm::normalize(movement)));
        if (jump)
        {
            Send(1, "apply_force", ApplyPhysics(m_TargetId, m_JumpImpulse, glm::vec3(0.f, 1.f, 0.f)));
            jump = false;
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
