#include "SceneNode.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"
#include "Settings.hpp"
#include "Scene.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {

    SceneNode::SceneNode(ActorId actorId, std::string name)
    {
        m_Parent = nullptr;
        m_ActorId = actorId;
        m_Name = name;
        m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
        SetTransform(nullptr);
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
                if (ev->space == Space::Local) TranslateLocal(ev->position);
                else if (ev->space == Space::Global) TranslateGlobal(ev->position);
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
                parsed = itr->second->OnActorEvent(e);
                itr++;
            }
        }
        return parsed;
    }

    void SceneNode::Update(Scene *scene, float deltaTime)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr).second->Update(scene, deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, glm::mat4 &parentTransform)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            glm::mat4 newTransform = parentTransform * m_LocalMatrix;
            if ((*itr).second->IsVisible(scene))
            {
                // TODO: Check alpha
                (*itr).second->Draw(scene, newTransform);
                (*itr).second->DrawChildren(scene, newTransform);
            }
        }
    }

    bool SceneNode::AddChild(SceneNodePtr child)
    {
        m_Children.emplace(child->GetName(), child);
        child->m_Parent = this;
        float new_radius = child->GetPosition().length() + child->GetRadius();
        if (new_radius > m_Radius)
            m_Radius = new_radius;
        return true;
    }

    SceneNodePtr SceneNode::GetChild(ActorId id)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            if (id == itr->second->GetActorId())
            {
                return itr->second;
            }
        }
        return SceneNodePtr();
    }

    SceneNodePtr SceneNode::GetChild(std::string name)
    {
        auto itr = m_Children.find(name);
        if (itr != m_Children.end())
        {
            return itr->second;
        }
        return SceneNodePtr();
    }

    bool SceneNode::RemoveChild(ActorId id)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
    	{
    		if (id == itr->second->GetActorId())
    		{
    			m_Children.erase(itr);
    			return true;
    		}
    	}
        return false;
    }

    bool SceneNode::RemoveChild(std::string name)
    {
        auto itr = m_Children.find(name);
        if (itr != m_Children.end())
        {
            m_Children.erase(itr);
            return true;
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

    void SceneNode::SetOrigin(glm::vec3 &origin) {
        m_Origin = origin;
    }

    void SceneNode::RotateGlobal(glm::vec3 &rotation)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Origin);
        if (rotation[0]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[0]), glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * GetGlobalMatrix()));
        if (rotation[1]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[1]), glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * GetGlobalMatrix()));
        if (rotation[2]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[2]), glm::vec3(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f) * GetGlobalMatrix()));
        m_LocalMatrix = glm::translate(m_LocalMatrix, -m_Origin);
    }

    void SceneNode::RotateLocal(glm::vec3 &rotation)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Origin);
        if (rotation[0]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        if (rotation[1]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        if (rotation[2]) m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        m_LocalMatrix = glm::translate(m_LocalMatrix, -m_Origin);
    }

    void SceneNode::RotateAround(float angle, glm::vec3 &dir)
    {
        m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angle), dir);
    }

    void SceneNode::TranslateGlobal(glm::vec3 &movement)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, glm::vec3(glm::vec4(movement, 0.0f) * GetGlobalMatrix()));
    }

    void SceneNode::TranslateLocal(glm::vec3 &movement)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, movement);
    }

    RootNode::RootNode() : SceneNode(INVALID_ID, "Root")
    {
        SceneNodePtr staticGroup(new SceneNode(INVALID_ID, "StaticGroup"));
        m_Children.emplace("staticGroup", staticGroup);
        SceneNodePtr actorGroup(new SceneNode(INVALID_ID, "ActorGroup"));
        m_Children.emplace("actorGroup", actorGroup);
        SceneNodePtr skyGroup(new SceneNode(INVALID_ID, "SkyGroup"));
        m_Children.emplace("skyGroup", skyGroup);
        SceneNodePtr invisibleGroup(new SceneNode(INVALID_ID, "InvisibleGroup"));
        m_Children.emplace("invisibleGroup", invisibleGroup);
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
                    m_Children["staticGroup"]->DrawChildren(scene, newMat);
                    break;
                case RenderPass::Actor:
                    m_Children["actorGroup"]->DrawChildren(scene, newMat);
                    break;
                case RenderPass::Sky:
                    view = scene->GetCamera()->GetViewAngle();
                    newMat = parentTransform * view;
                    Graphics_Engine::BeforeDrawSky();
                    m_Children["skyGroup"]->DrawChildren(scene, newMat);
                    Graphics_Engine::AfterDrawSky();
                    break;
            }
        }
    }

    bool RootNode::AddChild(SceneNodePtr child, RenderPass renderPass)
    {
        switch (renderPass)
        {
            case RenderPass::Static:
                return m_Children["staticGroup"]->AddChild(child);
            case RenderPass::Actor:
                return m_Children["actorGroup"]->AddChild(child);
            case RenderPass::Sky:
                return m_Children["skyGroup"]->AddChild(child);
            default:
                return m_Children["invisibleGroup"]->AddChild(child);
        }
    }

    Camera::Camera(ActorId actorId, std::string name) : SceneNode(actorId, name) {}

    const glm::mat4 Camera::GetView()
    {
        glm::mat4 matrix = GetGlobalMatrix();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    const glm::mat4 Camera::GetViewAngle()
    {
        glm::mat4 matrix = GetGlobalMatrix();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    MaterialNode::MaterialNode(ActorId actorId, std::string name, std::string shader, std::string texture) :
        SceneNode(actorId, name)
    {
        m_Texture = (texture != "") ? GET_RESOURCE(Texture, texture) : nullptr;
        m_Shader = GET_RESOURCE(Shader, shader);
    }

    void MaterialNode::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        (void)(scene); (void)(parentTransform);
        Graphics_Engine::UseShader(m_Shader);
        if (m_Texture) Graphics_Engine::BindTexture(m_Texture->GetId());
    }

    MeshNode::MeshNode(ActorId actorId, std::string name, std::string mesh) :
        SceneNode(actorId, name)
    {
        m_Mesh = GET_RESOURCE(MeshResource, mesh);
        m_Scale = glm::mat4(1.0f);
        m_Uv = glm::vec2(0.0f, 0.0f);
        m_TexSize = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void MeshNode::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        (void)(scene);
        glm::mat4 newMat = parentTransform * m_LocalMatrix * m_Scale;
        Graphics_Engine::GetShader()->Set("transform", newMat);
        Graphics_Engine::GetShader()->Set("uv", m_Uv);
        Graphics_Engine::GetShader()->Set("size", m_TexSize);
        Graphics_Engine::DrawMesh(m_Mesh);
    }

    void MeshNode::Scale(glm::vec3 &scale) {
        m_Origin.x *= scale.x;
        m_Origin.y *= scale.y;
        m_Origin.z *= scale.z;
        m_Scale = glm::scale(m_Scale, scale);
    }

    void MeshNode::Scale(float s) {
        m_Origin.x *= s;
        m_Origin.y *= s;
        m_Origin.z *= s;
        m_LocalMatrix = glm::scale(m_LocalMatrix, glm::vec3(s, s, s));
    }

    void MeshNode::SetUV(glm::vec3 &size, glm::vec2 &uv) {
        m_TexSize = size;
        m_Uv = uv;
    }
}
