#include "SceneNode.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Settings.hpp"
#include "Scene.hpp"
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
        m_OldPitch = 0.0f;
        m_OldYaw = 0.0f;
        m_OldRoll = 0.0f;
        m_Pitch = 0.0f;
        m_Yaw = 0.0f;
        m_Roll = 0.0f;
        m_Movement = glm::vec3(0, 0, 0);
    }

    bool SceneNode::OnActorEvent(ActorEvent *e)
    {
        bool parsed = false;
        if (e->name == m_Name)
        {
            if (e->GetType() == EventType::ActorMove)
            {
                ActorMoveEvent *ev = static_cast<ActorMoveEvent*>(e);
                m_Movement += glm::vec3(ev->position[0], ev->position[1],  ev->position[2]);
            } else if (e->GetType() == EventType::ActorRotate)
            {
                ActorRotateEvent *ev = static_cast<ActorRotateEvent*>(e);
                m_Yaw -= ev->rotation[1];
                m_Pitch -= ev->rotation[0];
                if (m_Pitch > 89.0f) m_Pitch = 89.0f;
                else if (m_Pitch < -89.0f) m_Pitch = -89.0f;
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

    void SceneNode::Update(Scene *scene, float deltaTime)
    {
        // glm::vec3 front, right, up;
        // front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
        // front.y = sin(glm::radians(m_Pitch));
        // front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
        // front = glm::normalize(front);
        // right = glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f));
        // right = glm::normalize(right);
        // up = glm::cross(right, front);
        // up = glm::normalize(up);

        m_ToWorld = glm::rotate(m_ToWorld, glm::radians(m_Roll - m_OldRoll), glm::vec3(0.0f, 0.0f, 1.0f));
        m_ToWorld = glm::rotate(m_ToWorld, glm::radians(m_Pitch - m_OldPitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec3 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * m_ToWorld;
        m_ToWorld = glm::rotate(m_ToWorld, glm::radians(m_Yaw - m_OldYaw), up);
        m_ToWorld = glm::translate(m_ToWorld, 5 * deltaTime * glm::vec3(m_Movement[0], m_Movement[1], m_Movement[2]));
        m_FromWorld = glm::transpose(m_ToWorld);

        m_OldYaw = m_Yaw;
        m_OldPitch = m_Pitch;
        m_OldRoll = m_Roll;

        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr)->Update(scene, deltaTime);
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
        {
            m_ToWorld = glm::mat4(1.0f);
            m_FromWorld = glm::mat4(1.0f);
        } else
        {
            m_ToWorld = *to;
            m_FromWorld = glm::transpose(m_ToWorld);
        }
    }

    glm::mat4 SceneNode::GetWorldMatrix()
    {
        if (m_Parent)
            return m_Parent->GetWorldMatrix() * m_FromWorld;
        else
            return m_FromWorld;
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
        glm::mat4 view = scene->GetCamera()->GetView();
        glm::mat4 new_transform = *parentTransform * view;
        for (int pass = RenderPass::Zero; pass < RenderPass::Last; pass++)
        {
            switch (pass)
            {
                case RenderPass::Static:
                case RenderPass::Actor:
                    m_Children[pass]->DrawChildren(scene, &new_transform);
                    break;
                case RenderPass::Sky:
                    view = scene->GetCamera()->GetViewAngle();
                    new_transform = *parentTransform * view;
                    Graphics_Engine::BeforeDrawSky();
                    m_Children[pass]->DrawChildren(scene, &new_transform);
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

    Camera::Camera(ActorId actorId, std::string name, glm::mat4 *to) : SceneNode(actorId, name, RenderPass::Actor, to) {}

    const glm::mat4 Camera::GetView()
    {
        glm::mat4 matrix = glm::transpose(GetWorldMatrix());
        glm::vec3 up = glm::vec3(matrix[1]);
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        return view;
    }

    const glm::mat4 Camera::GetViewAngle()
    {
        glm::mat4 matrix = glm::transpose(GetWorldMatrix());
        glm::vec3 up = glm::vec3(matrix[1]);
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        return view;
    }

    MeshNode::MeshNode(ActorId actorId, std::string name, RenderPass renderPass, glm::mat4 *to, std::string mesh, std::string shader, std::string texture) : SceneNode(actorId, name, renderPass, to)
    {
        if (texture != "") m_Texture = GET_RESOURCE(Texture, texture);
        m_Shader = GET_RESOURCE(Shader, shader);
        m_Mesh = GET_RESOURCE(MeshResource, mesh);
    }

    void MeshNode::Draw(Scene *scene, glm::mat4 *parentTransform)
    {
        (void)(scene);
        m_Shader->Use();
        glm::mat4 newMat = *parentTransform * m_ToWorld;
        m_Shader->Set("transform", &newMat);

        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
        Graphics_Engine::DrawMesh(m_Mesh);
    }

    Skybox::Skybox(std::string shader, std::string texture) : SceneNode(SKY_ID, "skybox", RenderPass::Sky)
    {
        if (texture != "") m_Texture = GET_RESOURCE(Texture, texture);
        m_Shader = GET_RESOURCE(Shader, shader);
        m_Mesh = GET_RESOURCE(MeshResource, "meshes/cube.mesh");
        m_Active = true;
    }

    void Skybox::Draw(Scene *scene, glm::mat4 *parentTransform)
    {
        (void)(scene);
        m_Shader->Use();
        m_Shader->Set("transform", parentTransform);

        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
        Graphics_Engine::DrawMesh(m_Mesh);
    }
}
