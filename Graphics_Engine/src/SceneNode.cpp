#pragma once
#include "SceneNode.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Settings.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {
    SceneNode::SceneNode(ActorId actorId, std::string name, RenderPass render_pass, const glm::mat4 *to)
    {
        m_Parent = nullptr;
        m_ActorId = actorId;
        m_Name = name;
        m_RenderPass = render_pass;
        SetTransform(to);
        SetRadius(0);
        m_Right = glm::vec3(m_ToWorld[0][0], m_ToWorld[1][0], m_ToWorld[2][0]);
        m_Front = glm::vec3(m_ToWorld[0][1], m_ToWorld[1][1], m_ToWorld[2][1]);
        m_Up = glm::vec3(m_ToWorld[0][2], m_ToWorld[1][2], m_ToWorld[2][2]);
        m_Position = glm::vec3(m_ToWorld[0][3], m_ToWorld[1][3], m_ToWorld[2][3]);
        m_Pitch = 0.0f;
        m_Yaw = -90.0f;
    }

    void SceneNode::Update(float deltaTime)
    {
        m_ToWorld[0] = glm::vec4(m_Right.x, m_Up.x, m_Front.x, m_Position.x * deltaTime);
        m_ToWorld[1] = glm::vec4(m_Right.y, m_Up.y, m_Front.y, m_Position.y * deltaTime);
        m_ToWorld[2] = glm::vec4(m_Right.z, m_Up.z, m_Front.z, m_Position.z * deltaTime);

        m_FromWorld[0] = glm::vec4(m_Right, m_FromWorld[0][3]);
        m_FromWorld[1] = glm::vec4(m_Up, m_FromWorld[1][3]);
        m_FromWorld[2] = glm::vec4(m_Front, m_FromWorld[2][3]);
        m_FromWorld[3] = glm::vec4(m_Position, m_FromWorld[3][3]) * deltaTime;

        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr)->Update(deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, glm::mat4 *parentTransform)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            glm::mat4 newTransform = *parentTransform * m_ToWorld;
            if ((*itr)->IsVisible(scene))
            {
                // TODO: Check alpha
                (*itr)->Draw(scene, &newTransform);
                (*itr)->DrawChildren(scene, &newTransform);
            }
        }
    }

    bool SceneNode::AddChild(SceneNodePtr child)
    {
        m_Children.push_back(child);
        child->m_Parent = this;
        float new_radius = child->GetPosition().length() + child->GetRadius();
        if (new_radius > m_Radius)
            m_Radius = new_radius;
        return true;
    }

    SceneNodePtr SceneNode::GetChild(ActorId id)
    {
        (void)(id);
        return SceneNodePtr();
    }

    bool SceneNode::RemoveChild(ActorId id)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
    	{
    		if (id == (*itr)->GetActorId())
    		{
    			m_Children.erase(itr);
    			return true;
    		}
    	}
        return false;
    }

    bool SceneNode::IsVisible(Scene *scene)
    {
        (void)(scene);
        return true;
    }

    void SceneNode::SetTransform(const glm::mat4 *to)
    {
        if (!to)
            m_ToWorld = glm::mat4(1.0f);
        else
            m_ToWorld = *to;
    }

    glm::mat4 SceneNode::GetWorldMatrix()
    {
        if (m_Parent)
            return m_Parent->GetWorldMatrix() * m_FromWorld;
        else
            return m_FromWorld;
    }

    bool SceneNode::OnActorEvent(ActorEvent *e)
    {
        bool parsed = false;
        if (e->name == m_Name)
        {
            if (e->GetType() == EventType::ActorMove)
            {
                ActorMoveEvent *ev = static_cast<ActorMoveEvent*>(e);
                m_Position += ev->position[0] * m_Front + ev->position[2] * m_Right;
            } else if (e->GetType() == EventType::ActorRotate)
            {
                ActorRotateEvent *ev = static_cast<ActorRotateEvent*>(e);
                m_Yaw -= ev->rotation[1];
                m_Pitch -= ev->rotation[0];
                if (m_Pitch > 89.0f) m_Pitch = 89.0f;
                else if (m_Pitch < -89.0f) m_Pitch = -89.0f;

                glm::vec3 front;
                // LOG_DEBUG("%s", glm::to_string(front).c_str());
                front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
                front.y = sin(glm::radians(m_Pitch));
                front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
                m_Front = glm::normalize(front);
                m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
                m_Up = glm::normalize(glm::cross(m_Right, m_Front));
            }

            parsed = true;
        } else {
            auto itr = m_Children.begin();
            while (!parsed && itr != m_Children.end())
            {
                parsed = (*itr)->OnActorEvent(e);
                itr++;
            }
        }
        return parsed;
    }

    RootNode::RootNode() : SceneNode(INVALID_ID, "Root", RenderPass::Zero)
    {
        m_Children.reserve(RenderPass::Last);
        SceneNodePtr staticGroup(new SceneNode(INVALID_ID, "StaticGroup", RenderPass::Static));
        m_Children.push_back(staticGroup);
        SceneNodePtr actorGroup(new SceneNode(INVALID_ID, "ActorGroup", RenderPass::Actor));
        m_Children.push_back(actorGroup);
        SceneNodePtr skyGroup(new SceneNode(INVALID_ID, "SkyGroup", RenderPass::Sky));
        m_Children.push_back(skyGroup);
        SceneNodePtr invisibleGroup(new SceneNode(INVALID_ID, "InvisibleGroup", RenderPass::NotRendered));
        m_Children.push_back(invisibleGroup);
    }

    void RootNode::DrawChildren(Scene *scene, glm::mat4 *parentTransform)
    {
        for (int pass = RenderPass::Zero; pass < RenderPass::Last; pass++)
        {
            switch (pass)
            {
                case RenderPass::Static:
                case RenderPass::Actor:
                    m_Children[pass]->DrawChildren(scene, parentTransform);
                    break;
                case RenderPass::Sky:
                    Graphics_Engine::BeforeDrawSky();
                    m_Children[pass]->DrawChildren(scene, parentTransform);
                    Graphics_Engine::AfterDrawSky();
                    break;
            }
        }
    }

    bool RootNode::AddChild(SceneNodePtr child)
    {
        RenderPass pass = child->GetRenderPass();
        if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
        {
            LOG_ERR("RootNode: Unexisting render pass.");
            return false;
        }

        return m_Children[pass]->AddChild(child);
    }

    Skybox::Skybox(std::string shader, std::string texture) : SceneNode(SKY_ID, "skybox", RenderPass::Sky)
    {
        if (texture != "")
        {
            m_Texture = GET_RESOURCE(Texture, texture);
        }
        m_Shader = GET_RESOURCE(Shader, shader);
        m_Mesh = GET_RESOURCE(MeshResource, "meshes/cube.mesh");
        m_Active = true;
    }

    void Skybox::Draw(Scene *scene, glm::mat4 *parentTransform)
    {
        m_Shader->Use();
        m_Shader->Set("transform", parentTransform);

        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
        Graphics_Engine::DrawMesh(m_Mesh);
    }

    Camera::Camera(ActorId actorId, std::string name, glm::mat4 *to) : SceneNode(actorId, name, RenderPass::Actor, to) {}

    const glm::mat4 Camera::GetView()
    {
        glm::mat4 matrix = GetWorldMatrix();
        glm::vec3 up = glm::vec3(matrix[1]);
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        return view;
    }

    // MeshNode::MeshNode(ActorId actorId, RenderPass renderPass, std::string name, glm::mat4 *to) : SceneNode(actorId, name, renderPass, to) {};
    //
    // MeshNode::Draw(Scene *scene, glm::mat4 parentTransform)
    // {
    //     Graphics_Engine::GetShader()->SetMat4("model", *parentTransform * m_ToWorld);
    // }
}
