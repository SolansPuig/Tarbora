#include "SceneNode.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Settings.hpp"
#include "Scene.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {

    SceneNode::SceneNode(ActorId actorId, std::string name, RenderPass render_pass, const glm::mat4 *matrix)
    {
        m_Parent = nullptr;
        m_ActorId = actorId;
        m_Name = name;
        m_RenderPass = render_pass;
        SetTransform(matrix);
        SetRadius(0);
    }

    bool SceneNode::OnActorEvent(ActorEvent *e)
    {
        bool parsed = false;
        if (e->name == m_Name)
        {
            if (e->GetType() == EventType::ActorMove)
            {
                ActorMoveEvent *ev = static_cast<ActorMoveEvent*>(e);
                if (ev->space == Space::Local) MoveLocal(ev->position);
                else if (ev->space == Space::Global) MoveGlobal(ev->position);
            } else if (e->GetType() == EventType::ActorRotate)
            {
                ActorRotateEvent *ev = static_cast<ActorRotateEvent*>(e);
                if (ev->space == Space::Local) RotateLocal(ev->rotation);
                else if (ev->space == Space::Global) RotateGlobal(ev->rotation);
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
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr)->Update(scene, deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, glm::mat4 &parentTransform)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            glm::mat4 newTransform = parentTransform * m_LocalMatrix;
            if ((*itr)->IsVisible(scene))
            {
                // TODO: Check alpha
                (*itr)->Draw(scene, newTransform);
                (*itr)->DrawChildren(scene, newTransform);
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

    void SceneNode::SetTransform(const glm::mat4 *matrix)
    {
        if (!matrix) m_LocalMatrix = glm::mat4(1.0f);
        else m_LocalMatrix = *matrix;
    }

    void SceneNode::SetPosition(const glm::vec3 &pos)
    {
        m_LocalMatrix[0][3] = pos.x;
        m_LocalMatrix[1][3] = pos.y;
        m_LocalMatrix[2][3] = pos.z;
    }

    const glm::vec3 SceneNode::GetPosition()
    {
        return glm::vec3( m_LocalMatrix[0][3], m_LocalMatrix[1][3], m_LocalMatrix[2][3]);
    }

    const glm::mat4 SceneNode::GetGlobalMatrix()
    {
        if (m_Parent)
            return m_Parent->GetGlobalMatrix() * m_LocalMatrix;
        else
            return m_LocalMatrix;
    }

    void SceneNode::RotateGlobal(glm::vec3 &rotation)
    {
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[0]), glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * GetGlobalMatrix()));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[1]), glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * GetGlobalMatrix()));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[2]), glm::vec3(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * GetGlobalMatrix()));
    }

    void SceneNode::RotateLocal(glm::vec3 &rotation)
    {
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void SceneNode::RotateAround(float angle, glm::vec3 &dir)
    {
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angle), dir);
    }

    void SceneNode::MoveGlobal(glm::vec3 &movement)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, glm::vec3(glm::vec4(movement, 0.0f) * GetGlobalMatrix()));
    }

    void SceneNode::MoveLocal(glm::vec3 &movement)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, movement);
    }

    void SceneNode::MoveTo(glm::vec3 &dir)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, dir);
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

    void RootNode::DrawChildren(Scene *scene, glm::mat4 &parentTransform)
    {
        glm::mat4 view = scene->GetCamera()->GetView();
        glm::mat4 newMat = parentTransform * view;
        for (int pass = RenderPass::Zero; pass < RenderPass::Last; pass++)
        {
            switch (pass)
            {
                case RenderPass::Static:
                case RenderPass::Actor:
                    m_Children[pass]->DrawChildren(scene, newMat);
                    break;
                case RenderPass::Sky:
                    view = scene->GetCamera()->GetViewAngle();
                    newMat = parentTransform * view;
                    Graphics_Engine::BeforeDrawSky();
                    m_Children[pass]->DrawChildren(scene, newMat);
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

    Camera::Camera(ActorId actorId, std::string name, glm::mat4 *matrix) : SceneNode(actorId, name, RenderPass::Actor, matrix) {}

    const glm::mat4 Camera::GetView()
    {
        glm::mat4 matrix = GetGlobalMatrix();
        glm::vec3 up = glm::vec3(matrix[1]);
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        return view;
    }

    const glm::mat4 Camera::GetViewAngle()
    {
        glm::mat4 matrix = GetGlobalMatrix();
        glm::vec3 up = glm::vec3(matrix[1]);
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::mat4 view = glm::lookAt(position, position + front, up);
        return view;
    }

    MeshNode::MeshNode(ActorId actorId, std::string name, RenderPass renderPass, glm::mat4 *matrix, std::string mesh, std::string shader, std::string texture) : SceneNode(actorId, name, renderPass, matrix)
    {
        m_Texture = (texture != "") ? GET_RESOURCE(Texture, texture) : nullptr;
        m_Shader = GET_RESOURCE(Shader, shader);
        m_Mesh = GET_RESOURCE(MeshResource, mesh);
    }

    void MeshNode::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        (void)(scene);
        m_Shader->Use();
        glm::mat4 newMat = parentTransform * m_LocalMatrix;
        m_Shader->Set("transform", newMat);

        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
        Graphics_Engine::DrawMesh(m_Mesh);
    }

    Skybox::Skybox(std::string shader, std::string texture) : SceneNode(SKY_ID, "skybox", RenderPass::Sky)
    {
        m_Texture = (texture != "") ? GET_RESOURCE(Texture, texture) : nullptr;
        m_Shader = GET_RESOURCE(Shader, shader);
        m_Mesh = GET_RESOURCE(MeshResource, "meshes/cube.mesh");
        m_Active = true;
    }

    void Skybox::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        (void)(scene);
        m_Shader->Use();
        m_Shader->Set("transform", parentTransform);

        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
        Graphics_Engine::DrawMesh(m_Mesh);
    }
}
