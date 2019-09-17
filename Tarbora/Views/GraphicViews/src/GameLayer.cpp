#include "../inc/GameLayer.hpp"

namespace Tarbora {
    GameLayerImpl::GameLayerImpl(GraphicView *view, bool start_active) :
        Layer(view, start_active)
    {
        m_Scene = std::unique_ptr<Scene>(new Scene(m_View));

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

        m_CreateActor = m_View->GetMessageManager()->Subscribe("create_actor_model", [this](std::string subject, std::string body)
        {
            LOG_DEBUG("create_actor_model");
            CreateActorMessage m;
            m.ParseFromString(body);
            if (m_Scene->GetChild(m.id()) != nullptr)
            {
                m_Scene->RemoveChild(m.id());
            }
            m_Scene->CreateActorModel(m.id(), m.entity(), m.variant());
        });

        m_SetCamera = m_View->GetMessageManager()->Subscribe("set_camera", [this](std::string subject, std::string body)
        {
            LOG_DEBUG("set_camera");
            SetCameraMessage m;
            m.ParseFromString(body);
            m_Scene->SetCamera(m.id(), m.name());
        });

        m_MoveActor = m_View->GetMessageManager()->Subscribe("move_actor", [this](std::string subject, std::string body)
        {
            MoveActorMessage m;
            m.ParseFromString(body);
            std::shared_ptr<SceneNode> actor = m_Scene->GetChild(m.id());
            actor->SetPosition(Vec3toGLM(m.position()));
            actor->SetRotationMatrix(Mat3toGLM(m.rotation()));
        });

        m_DeleteActor = m_View->GetMessageManager()->Subscribe("delete_actor", [this](std::string subject, std::string body)
        {
            DeleteActorMessage m;
            m.ParseFromString(body);
            m_Scene->RemoveChild(m.id());
        });

        m_ActorAnimation = m_View->GetMessageManager()->Subscribe("animate_actor", [this](std::string subject, std::string body)
        {
            AnimateActorMessage m;
            m.ParseFromString(body);
            m_Scene->AnimateActor(m.id(), m.animation());
        });
    }

    GameLayerImpl::~GameLayerImpl()
    {
        m_View->GetMessageManager()->Desubscribe("create_actor_model", m_CreateActor);
        m_View->GetMessageManager()->Desubscribe("set_camera", m_SetCamera);
        m_View->GetMessageManager()->Desubscribe("move_actor", m_MoveActor);
        m_View->GetMessageManager()->Desubscribe("delete_actor", m_DeleteActor);
        m_View->GetMessageManager()->Desubscribe("animate_actor", m_ActorAnimation);
    }

    bool GameLayerImpl::OnMessage(Message *e)
    {
        return false;
    }

    void GameLayerImpl::GetInput()
    {
        static glm::vec3 movement(0.f, 0.f, 0.f);
        static bool jump = false;

        if (m_View->Input()->GetKeyDown(KEY_W))
        {
            movement.z += 1;
        }
        if (m_View->Input()->GetKeyUp(KEY_W))
        {
            movement.z -= 1;
        }
        if (m_View->Input()->GetKeyDown(KEY_S))
        {
            movement.z -= 1;
        }
        if (m_View->Input()->GetKeyUp(KEY_S))
        {
            movement.z += 1;
        }
        if (m_View->Input()->GetKeyDown(KEY_A))
        {
            movement.x += 1;
        }
        if (m_View->Input()->GetKeyUp(KEY_A))
        {
            movement.x -= 1;
        }
        if (m_View->Input()->GetKeyDown(KEY_D))
        {
            movement.x -= 1;
        }
        if (m_View->Input()->GetKeyUp(KEY_D))
        {
            movement.x += 1;
        }
        if (m_View->Input()->GetKeyDown(KEY_SPACE))
        {
            if (!jump) jump = true;
        }
        if (m_View->Input()->GetKeyDown(KEY_X))
        {
            CreateActor(m_View, "cube.json", "", glm::vec3(0.f,5.f,-5.f));
        }
        if (m_View->Input()->GetKeyDown(KEY_Y))
        {
            static bool thirdPerson = false;
            LOG_DEBUG("Third person %d", thirdPerson);
            if (thirdPerson) m_Scene->SetCamera(5, "1st_person");
            else m_Scene->SetCamera(5, "3rd_person");

            thirdPerson = !thirdPerson;
        }


        SetVelocity(m_View, m_TargetId, m_DirectionImpulse * glm::normalize(movement));
        if (jump)
        {
            ApplyForce(m_View, m_TargetId, m_JumpImpulse, glm::vec3(0.f, 1.f, 0.f));
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