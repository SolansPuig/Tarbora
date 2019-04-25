#pragma once
#include "../../Framework/Framework.hpp"
#include "Scene.hpp"
#include "Skybox.hpp"
#include "Layer.hpp"

namespace Tarbora {
    class GameLayer : public Layer
    {
    public:
        GameLayer(bool start_active=true) : Layer(start_active)
        {
            m_Skybox.reset(new Skybox(m_Scene, "shaders/sky.shader.json", "textures/sky.png"));
            m_Movement = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Jump = false;
        }

        ~GameLayer() {}

        virtual bool OnEvent(Event *e) override
        {
            if (e->GetType() == EventType::MouseMove)
            {
                MouseMoveEvent *ev = static_cast<MouseMoveEvent*>(e);
                static bool firstMouse = true;
                if (firstMouse)
                {
                    m_LastX = ev->x;
                    m_LastY = ev->y;
                    firstMouse = false;
                }

                float xoffset = ev->x - m_LastX;
                float yoffset = ev->y - m_LastY;
                m_LastX = ev->x;
                m_LastY = ev->y;

                m_Rotation[0] += yoffset;

                m_Rotation[1] -= xoffset;

                return true;
            } else if (e->GetType() == EventType::KeyPress)
            {
                KeyPressEvent *ev = static_cast<KeyPressEvent*>(e);
                if (ev->repeat == 0)
                {
                    switch (ev->key)
                    {
                        case KEY_W:
                            m_Movement[2] -= 1;
                            break;
                        case KEY_S:
                            m_Movement[2] += 1;
                            break;
                        case KEY_A:
                            m_Movement[0] -= 1;
                            break;
                        case KEY_D:
                            m_Movement[0] += 1;
                            break;
                        case KEY_SPACE:
                            m_Jump = true;
                            break;
                    }
                }
                return true;
            } else if (e->GetType() == EventType::KeyRelease)
            {
                KeyReleaseEvent *ev = static_cast<KeyReleaseEvent*>(e);
                switch (ev->key)
                {
                    case KEY_W:
                        m_Movement[2] += 1;
                        break;
                    case KEY_S:
                        m_Movement[2] -= 1;
                        break;
                    case KEY_A:
                        m_Movement[0] += 1;
                        break;
                    case KEY_D:
                        m_Movement[0] -= 1;
                        break;
                }
                return true;
            }
            return false;
        }

        void Update(float deltaTime) override
        {
            if (m_Movement != glm::vec3(0.0f, 0.0f, 0.0f))
            {
                ApplyForceEvent ev = ApplyForceEvent(m_TargetId, 15, glm::normalize(m_Movement));
                EventManager::Trigger(ApplyForce, &ev);
            }
            if (m_Rotation != glm::vec3(0.0f, 0.0f, 0.0f))
            {
                ApplyTorqueEvent ev = ApplyTorqueEvent(m_TargetId, 1, glm::normalize(m_Rotation));
                EventManager::Trigger(ApplyTorque, &ev);
            }
            if (m_Jump)
            {
                ApplyForceEvent ev = ApplyForceEvent(m_TargetId, 500, glm::vec3(0.0f, 1.0f, 0.0f));
                EventManager::Trigger(ApplyForce, &ev);
                m_Jump = false;
            }

            m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Scene.Update(deltaTime);
        }

        void Draw() override
        {
            m_Scene.Draw();
        }

        SkyboxPtr GetSkybox() const { return m_Skybox; }

        void SetTargetId(ActorId id) { m_TargetId = id; }
        ActorId GetTargetId() const { return m_TargetId; }
    private:
        Scene m_Scene;
        ActorId m_TargetId;
        float m_LastX, m_LastY; // Last mouse position
        glm::vec3 m_Rotation;
        glm::vec3 m_Movement;
        SkyboxPtr m_Skybox;
        bool m_Jump;
    };
}
