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

                float sensitivity = 0.02;
                EventManager::Trigger(ActorRotate, new ActorRotateEvent(m_TargetId, "body", glm::vec3(-yoffset * sensitivity, xoffset * sensitivity, 0)));
                return true;
            } else if (e->GetType() == EventType::KeyPress)
            {
                KeyPressEvent *ev = static_cast<KeyPressEvent*>(e);
                if (ev->repeat == 0)
                {
                    switch (ev->key)
                    {
                        case KEY_W:
                            EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(0, 0, 1)));
                            break;
                        case KEY_S:
                            EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(0, 0, -1)));
                            break;
                        case KEY_A:
                            EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(1, 0, 0)));
                            break;
                        case KEY_D:
                            EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(-1, 0, 0)));
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
                        EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(0, 0, -1)));
                        break;
                    case KEY_S:
                        EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(0, 0, 1)));
                        break;
                    case KEY_A:
                        EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(-1, 0, 0)));
                        break;
                    case KEY_D:
                        EventManager::Trigger(ActorMove, new ActorMoveEvent(m_TargetId, "body", glm::vec3(1, 0, 0)));
                        break;
                }
                return true;
            }
            return false;
        }

        void Update(float deltaTime) override
        {
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
        std::shared_ptr<Skybox> m_Skybox;
    };
}
