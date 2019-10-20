#include "../inc/Scene.hpp"
#include "../../../Messages/BasicMessages.hpp"

namespace Tarbora {
    SceneNode::SceneNode(ActorId actorId, std::string name)
    {
        m_Parent = nullptr;
        m_ActorId = actorId;
        m_Name = name;
        m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
        SetTransform(nullptr);
        SetRadius(0);

        m_OldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_TargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        m_PositionCounter = 0.0f;
        m_PositionTime = 0.0f;
        m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_OldRotation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_TargetRotation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_RotationCounter = 0.0f;
        m_RotationTime = 0.0f;
        m_OldScale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_TargetScale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_ScaleCounter = 0.0f;
        m_ScaleTime = 0.0f;
    }

    void SceneNode::Update(Scene *scene, float deltaTime)
    {
        if (m_Scale != m_TargetScale)
        {
            m_ScaleCounter += deltaTime;
            if (InterpolateScale(m_ScaleCounter/m_ScaleTime))
            {
                m_ScaleCounter = 0.0f;
            }
        }

        if (m_Rotation != m_TargetRotation)
        {
            m_RotationCounter += deltaTime;
            InterpolateRotation(m_RotationCounter/m_RotationTime);
        }


        if (m_Position != m_TargetPosition)
        {
            m_PositionCounter += deltaTime;
            InterpolatePosition(m_PositionCounter/m_PositionTime);
        }

        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr).second->Update(scene, deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, const glm::mat4 &parentTransform)
    {
        glm::mat4 transform = parentTransform * m_LocalMatrix;
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            if (itr->second->IsVisible(scene))
            {
                // TODO: Check alpha
                itr->second->Draw(scene, transform);
                itr->second->DrawChildren(scene, transform);
                itr->second->AfterDraw(scene);
            }
        }
    }

    bool SceneNode::AddChild(SceneNodePtr child)
    {
        m_Children[child->GetName()] = child;
        child->m_Parent = this;
        float new_radius = child->GetPosition().length() + child->GetRadius();
        if (new_radius > m_Radius)
            m_Radius = new_radius;
        return true;
    }

    SceneNodePtr SceneNode::GetChild(ActorId id)
    {
        std::string name = std::to_string(id);
        auto itr = m_Children.find(name);
        if (itr != m_Children.end())
        {
            return itr->second;
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
        std::string name = std::to_string(id);
        auto itr = m_Children.find(name);
        if (itr != m_Children.end())
        {
            m_Children.erase(itr);
            return true;
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

    void SceneNode::SetPosition(const glm::vec3 &position)
    {
        m_LocalMatrix[3] = glm::vec4(position.x, position.y, position.z, 1.0f);
        m_Position = position;
        m_OldPosition = m_Position;
        m_TargetPosition = m_Position;
    }

    void SceneNode::Move(const glm::vec3 &vector)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, vector);
        m_Position += vector;
        m_OldPosition = m_Position;
        m_TargetPosition = m_Position;
    }

    void SceneNode::MoveTo(const glm::vec3 &position, float timeToComplete)
    {
        if (timeToComplete > 0.0f)
        {
            m_PositionCounter = 0.0f;
            m_OldPosition = m_Position;
            m_TargetPosition = position;
            m_PositionTime = timeToComplete;
        }
        else
        {
            SetPosition(position);
        }
    }

    bool SceneNode::InterpolatePosition(float fraction)
    {
        if (fraction >= 1.0f)
        {
            SetPosition(m_TargetPosition);
            return true;
        }

        glm::vec3 position = m_OldPosition + fraction*(m_TargetPosition - m_OldPosition);
        m_LocalMatrix = glm::translate(m_LocalMatrix, (position - m_Position));
        m_Position = position;

        return false;
    }

    void SceneNode::SetRotation(const glm::vec3 &rotation)
    {
        Rotate(glm::vec3(rotation.x - m_Rotation.x, rotation.y - m_Rotation.y, rotation.z - m_Rotation.z));
    }

    void SceneNode::Rotate(const glm::vec3 &angles)
    {
        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Origin);
        if (angles.x)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (angles.y)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (angles.z)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        m_LocalMatrix = glm::translate(m_LocalMatrix, -m_Origin);

        m_Rotation += angles;
        m_OldRotation = m_Rotation;
        m_TargetRotation = m_Rotation;
    }

    void SceneNode::RotateTo(const glm::vec3 &targetRotation, float timeToComplete)
    {
        if (timeToComplete > 0.0f)
        {
            m_RotationCounter = 0.0f;
            m_OldRotation = m_Rotation;
            m_TargetRotation = targetRotation;
            m_RotationTime = timeToComplete;

            for (int i = 0; i < 3; i++)
            {
                m_TargetRotation[i] = (((((int)m_TargetRotation[i] - (int)m_OldRotation[i]) % 360) + 540) % 360) - 180;
            }
        }
        else
        {
            SetRotation(targetRotation);
        }
    }

    bool SceneNode::InterpolateRotation(float fraction)
    {
        if (fraction >= 1.0f)
        {
            m_OldRotation = m_Rotation;
            return true;
        }
        m_LocalMatrix = glm::translate(m_LocalMatrix, m_Origin);

        glm::vec3 rotation = m_OldRotation + fraction*(m_TargetRotation);
        if (rotation.x)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation.x - m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (rotation.y)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation.y - m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (rotation.z)
        {
            m_LocalMatrix = glm::rotate(m_LocalMatrix, glm::radians(rotation.z - m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        m_Rotation = rotation;
        m_LocalMatrix = glm::translate(m_LocalMatrix, -m_Origin);

        return false;
    }

    void SceneNode::SetRotationMatrix(const glm::mat3 &rotation)
    {
        glm::mat3 newrot = glm::transpose(rotation);
        m_LocalMatrix[0] = glm::vec4(newrot[0][0], newrot[1][0], newrot[2][0], m_LocalMatrix[0][3]);
        m_LocalMatrix[1] = glm::vec4(newrot[0][1], newrot[1][1], newrot[2][1], m_LocalMatrix[1][3]);
        m_LocalMatrix[2] = glm::vec4(newrot[0][2], newrot[1][2], newrot[2][2], m_LocalMatrix[2][3]);
    }

    void SceneNode::SetScale(const glm::vec3 &scale)
    {
        Scale(glm::vec3(scale.x / m_Scale.x, scale.y / m_Scale.y, scale.z / m_Scale.z));
    }

    void SceneNode::SetGlobalScale(const glm::vec3 &scale)
    {
        m_LocalMatrix = glm::scale(m_LocalMatrix, scale);

        m_Origin.x *= scale.x;
        m_Origin.y *= scale.y;
        m_Origin.z *= scale.z;
    }

    void SceneNode::Scale(const glm::vec3 &scale)
    {
        m_Scale *= scale;
        m_OldScale = m_Scale;
        m_TargetScale = m_Scale;

        m_Origin.x *= scale.x;
        m_Origin.y *= scale.y;
        m_Origin.z *= scale.z;
    }

    void SceneNode::ScaleTo(const glm::vec3 &scale, float timeToComplete)
    {
        m_TargetScale = scale;
        m_ScaleTime = timeToComplete;
    }

    bool SceneNode::InterpolateScale(float fraction)
    {
        if (fraction >= 1.0f)
        {
            SetScale(m_TargetScale);
            return true;
        }
        m_Scale = m_OldScale + fraction*(m_TargetScale - m_OldScale);

        m_Origin.x /= m_OldScale.x;
        m_Origin.y /= m_OldScale.y;
        m_Origin.z /= m_OldScale.z;

        m_Origin.x *= m_Scale.x;
        m_Origin.y *= m_Scale.y;
        m_Origin.z *= m_Scale.z;

        return false;
    }

    void SceneNode::SetTransform(glm::mat4 *matrix)
    {
        if (!matrix) m_LocalMatrix = glm::mat4(1.0f);
        else m_LocalMatrix = *matrix;
    }

    const glm::mat4 SceneNode::GetGlobalTransform()
    {
        if (m_Parent)
            return m_Parent->GetGlobalTransform() * m_LocalMatrix;
        else
            return m_LocalMatrix;
    }

    void SceneNode::SetOrigin(const glm::vec3 &origin) {
        m_Origin = origin;
    }

    RootNode::RootNode() : SceneNode(INVALID_ID, "Root") {}

    Camera::Camera(ActorId actorId, std::string name) : SceneNode(actorId, name) {}

    const glm::mat4 Camera::GetView()
    {
        glm::mat4 matrix = GetGlobalTransform();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    const glm::mat4 Camera::GetViewAngle()
    {
        glm::mat4 matrix = GetGlobalTransform();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    const glm::vec3 Camera::GetViewPosition()
    {
        glm::mat4 matrix = GetGlobalTransform();
        glm::vec3 position = glm::vec3(matrix[3]);
        return position;
    }

    MaterialNode::MaterialNode(ActorId actorId, std::string name, std::string material) :
        SceneNode(actorId, name)
    {
        m_Material = ResourcePtr<Material>("materials/" + material);
    }

    void MaterialNode::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        (void)(parentTransform);
        scene->GetRenderQueue()->PushMaterial(m_Material);
    }

    void MaterialNode::AfterDraw(Scene *scene)
    {
        scene->GetRenderQueue()->PopMaterial();
    }

    MeshNode::MeshNode(ActorId actorId, std::string name, RenderPass renderPass, std::string mesh) :
        SceneNode(actorId, name), m_RenderPass(renderPass)
    {
        m_Mesh = ResourcePtr<Mesh>("meshes/" + mesh);
        if (m_Mesh == nullptr) m_Mesh = ResourcePtr<Mesh>("meshes/cube.mesh");
        m_Uv = glm::vec2(0.0f, 0.0f);
        m_TextureSize = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void MeshNode::Draw(Scene *scene, glm::mat4 &parentTransform)
    {
        if (m_Mesh != nullptr)
        {
            glm::mat4 transform = parentTransform * glm::scale(m_LocalMatrix, m_Scale);
            scene->GetRenderQueue()->DrawMesh(m_RenderPass, m_Mesh, transform, m_Uv, m_TextureSize);
        }
    }

    void MeshNode::SetUV(glm::vec3 &size, glm::vec2 &uv) {
        m_TextureSize = size;
        m_Uv = uv;
    }
}
