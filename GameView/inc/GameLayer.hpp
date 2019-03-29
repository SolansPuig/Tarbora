#pragma once
#include "Scene.hpp"
#include "KeyCodes.hpp"
#include <memory>

namespace Tarbora {
    class GameLayer : public Layer
    {
    public:
        GameLayer(bool start_active=true) : Layer(start_active)
        {
            m_Skybox.reset(new Skybox("shaders/sky.shader.json", "textures/sky.png"));
            m_Scene.AddChild(m_Skybox);
            m_Movement = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        ~GameLayer() {}

        virtual bool OnEvent(Event *e) override
        {
            float sensitivity = 0.8;
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

                m_Rotation[0] += yoffset * sensitivity;
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
            EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", deltaTime * m_Movement, Space::Local));
            if (m_Rotation[0] != 0.0f) EventManager::Trigger(ActorRotate, new ActorRotateEvent(m_TargetId, "body", deltaTime * glm::vec3(m_Rotation[0], 0.0f, 0.0f), Space::Local));
            if (m_Rotation[1] != 0.0f) EventManager::Trigger(ActorRotate, new ActorRotateEvent(m_TargetId, "body", deltaTime * glm::vec3(0.0f, m_Rotation[1], 0.0f), Space::Global));
            m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            m_Scene.Update(deltaTime);
        }

        void Draw() override
        {
            m_Scene.Draw();
        }

        std::shared_ptr<Skybox> GetSkybox() const { return m_Skybox; }

        void SetTargetId(ActorId id) { m_TargetId = id; }
        ActorId GetTargetId() const { return m_TargetId; }
    private:
        Scene m_Scene;
        ActorId m_TargetId;
        float m_LastX, m_LastY; // Last mouse position
        glm::vec3 m_Rotation;
        glm::vec3 m_Movement;
        std::shared_ptr<Skybox> m_Skybox;
    };
}
