#pragma once
#include "Scene.hpp"
#include "KeyCodes.hpp"
#include "Skybox.hpp"
#include <memory>

namespace Tarbora {
    class GameLayer : public Layer
    {
    public:
        GameLayer(bool start_active=true) : Layer(start_active)
        {
            m_Skybox.reset(new Skybox(m_Scene, "shaders/sky.shader.json", "textures/sky.png"));
            m_Movement = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Pitch = 0;
        }

        ~GameLayer() {}

        virtual bool OnEvent(Event *e) override
        {
            float sensitivity = 0.05f;
            float speed = 5;
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

                if ((m_Pitch + yoffset * sensitivity) < 89 && (m_Pitch + yoffset * sensitivity) > -89)
                {
                    m_Pitch += yoffset * sensitivity;
                    m_Rotation[0] += yoffset * sensitivity;
                }

                m_Rotation[1] -= xoffset * sensitivity;

                return true;
            } else if (e->GetType() == EventType::KeyPress)
            {
                KeyPressEvent *ev = static_cast<KeyPressEvent*>(e);
                if (ev->repeat == 0)
                {
                    switch (ev->key)
                    {
                        case KEY_W:
                            m_Movement[2] += speed;
                            break;
                        case KEY_S:
                            m_Movement[2] -= speed;
                            break;
                        case KEY_A:
                            m_Movement[0] += speed;
                            break;
                        case KEY_D:
                            m_Movement[0] -= speed;
                            break;
                        case KEY_C:
                            if (m_TargetId == 1) m_TargetId = 15;
                            else if (m_TargetId == 15) m_TargetId = 1;
                    }
                }
                return true;
            } else if (e->GetType() == EventType::KeyRelease)
            {
                KeyReleaseEvent *ev = static_cast<KeyReleaseEvent*>(e);
                switch (ev->key)
                {
                    case KEY_W:
                        m_Movement[2] -= speed;
                        break;
                    case KEY_S:
                        m_Movement[2] += speed;
                        break;
                    case KEY_A:
                        m_Movement[0] -= speed;
                        break;
                    case KEY_D:
                        m_Movement[0] += speed;
                        break;
                }
                return true;
            }
            return false;
        }

        void Update(float deltaTime) override
        {
            if (m_Movement != glm::vec3(0.0f, 0.0f, 0.0f))
                EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", deltaTime * m_Movement, Space::Local));
            if (m_Rotation[0] != 0.0f)
                EventManager::Trigger(ActorRotate, new ActorRotateEvent(m_TargetId, "body", glm::vec3(m_Rotation[0], 0.0f, 0.0f), Space::Local));
            if (m_Rotation[1] != 0.0f)
                EventManager::Trigger(ActorRotate, new ActorRotateEvent(m_TargetId, "body", glm::vec3(0.0f, m_Rotation[1], 0.0f), Space::Global));
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
        float m_Pitch;
        glm::vec3 m_Movement;
        SkyboxPtr m_Skybox;
    };
}
