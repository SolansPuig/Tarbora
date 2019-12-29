#include "../inc/Scene.hpp"
#include "../../../Messages/BasicMessages.hpp"

namespace Tarbora {
    SceneNode::SceneNode(ActorId actorId, const std::string &name)
    {
        m_Parent = nullptr;
        m_ActorId = actorId;
        m_Name = name;
        m_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Radius = 0.0f;
        m_Transformation = glm::mat4(1.0f);
        m_Deformation = glm::mat4(1.0f);

        m_Properties["position"] = PropertyPtr(new Position(glm::vec3(0.0f, 0.0f, 0.0f), &m_Transformation));
        m_Properties["rotation"] = PropertyPtr(new Rotation(glm::vec3(0.0f, 0.0f, 0.0f), &m_Transformation, &m_Origin));
        m_Properties["scale"] = PropertyPtr(new Scale(glm::vec3(1.0f, 1.0f, 1.0f), &m_Deformation, &m_Origin));
    }

    void SceneNode::Update(Scene *scene, float deltaTime)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr).second->Update(scene, deltaTime);
        for (auto itr = m_Properties.begin(); itr != m_Properties.end(); itr++)
            (*itr).second->Update(deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, const glm::mat4 &parentTransform)
    {
        glm::mat4 transform = parentTransform * m_Transformation;
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
        float new_radius = child->Get("position").length() + child->GetRadius();
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

    SceneNodePtr SceneNode::GetChild(const std::string &name)
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

    bool SceneNode::RemoveChild(const std::string &name)
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
        m_Transformation[3] = glm::vec4(position.x, position.y, position.z, 1.0f);
    }

    void SceneNode::SetRotationMatrix(const glm::mat3 &rotation)
    {
        glm::mat3 newrot = glm::transpose(rotation);
        m_Transformation[0] = glm::vec4(newrot[0][0], newrot[1][0], newrot[2][0], m_Transformation[0][3]);
        m_Transformation[1] = glm::vec4(newrot[0][1], newrot[1][1], newrot[2][1], m_Transformation[1][3]);
        m_Transformation[2] = glm::vec4(newrot[0][2], newrot[1][2], newrot[2][2], m_Transformation[2][3]);
    }

    void SceneNode::SetTransform(const glm::mat4 &matrix)
    {
        m_Transformation = matrix;
    }

    const glm::mat4 SceneNode::GetGlobalTransform()
    {
        if (m_Parent)
            return m_Parent->GetGlobalTransform() * m_Transformation;
        else
            return m_Transformation;
    }

    void SceneNode::Set(const std::string &name, const glm::vec3 &value)
    {
        m_Properties[name]->Set(value);
    }

    void SceneNode::Add(const std::string &name, const glm::vec3 &value)
    {
        m_Properties[name]->Add(value);
    }

    void SceneNode::InterpolateTo(const std::string &name, const glm::vec3 &value, float timeToComplete)
    {
        PropertyPtr property = m_Properties[name];
        if (!property->IsAnimatable()) {
            property = property->MakeAnimatable();
            m_Properties[name] = property;
        }
        property->InterpolateTo(value, timeToComplete);
    }

    const glm::vec3 &SceneNode::Get(const std::string &name)
    {
        return m_Properties[name]->Get();
    }

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

    MaterialNode::MaterialNode(ActorId actorId, const std::string &name, const std::string &material) :
        SceneNode(actorId, name)
    {
        m_Material = ResourcePtr<Material>("materials/" + material, "materials/missing.mat.lua");
    }

    void MaterialNode::Draw(Scene *scene, const glm::mat4 &parentTransform)
    {
        (void)(parentTransform);
        scene->GetRenderQueue()->PushMaterial(m_Material);
    }

    void MaterialNode::AfterDraw(Scene *scene)
    {
        scene->GetRenderQueue()->PopMaterial();
    }

    MeshNode::MeshNode(ActorId actorId, const std::string &name, RenderPass renderPass, const std::string &mesh) :
        SceneNode(actorId, name), m_RenderPass(renderPass)
    {
        m_Mesh = ResourcePtr<Mesh>("meshes/" + mesh, "meshes/cube.mesh");

        m_Uv = glm::vec2(0.0f, 0.0f);
        m_TextureSize = glm::vec3(0.0f, 0.0f, 0.0f);

        m_Properties["color_primary"] = PropertyPtr(new NodeProperty(glm::vec3(0.0f, 0.0f, 0.0f)));
        m_Properties["color_secondary"] = PropertyPtr(new NodeProperty(glm::vec3(0.0f, 0.0f, 0.0f)));
        m_Properties["color_detail1"] = PropertyPtr(new NodeProperty(glm::vec3(0.0f, 0.0f, 0.0f)));
        m_Properties["color_detail2"] = PropertyPtr(new NodeProperty(glm::vec3(0.0f, 0.0f, 0.0f)));
    }

    void MeshNode::Draw(Scene *scene, const glm::mat4 &parentTransform)
    {
        if (m_Mesh != nullptr)
        {
            glm::mat4 transform = parentTransform * m_Transformation * m_Deformation;
            scene->GetRenderQueue()->DrawMesh(
                m_RenderPass,
                m_Mesh,
                transform,
                m_Uv,
                m_TextureSize,
                m_Properties["color_primary"]->Get(),
                m_Properties["color_secondary"]->Get(),
                m_Properties["color_detail1"]->Get(),
                m_Properties["color_detail2"]->Get()
            );
        }
    }

    void MeshNode::SetUV(const glm::vec3 &size, const glm::vec2 &uv) {
        m_TextureSize = size;
        m_Uv = uv;
    }

    // void MeshNode::SetShear(const glm::vec3 &shearA, const glm::vec3 &shearB)
    // {
    //     Shear(
    //         glm::vec3(shearA.x / m_ShearA.x, shearA.y / m_ShearA.y, shearA.z / m_ShearA.z),
    //         glm::vec3(shearB.x / m_ShearB.x, shearB.y / m_ShearB.y, shearB.z / m_ShearB.z)
    //     );
    // }

    // void MeshNode::Shear(const glm::vec3 &shearA, const glm::vec3 &shearB)
    // {
    //     m_DeformationMatrix = glm::shearX3D(m_DeformationMatrix, shearA.x, shearB.x);
    //     m_DeformationMatrix = glm::shearY3D(m_DeformationMatrix, shearA.y, shearB.y);
    //     m_DeformationMatrix = glm::shearZ3D(m_DeformationMatrix, shearA.z, shearB.z);
    //
    //     m_ShearA *= shearA;
    //     m_ShearB *= shearB;
    // }
}
