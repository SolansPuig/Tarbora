#include "Scene.hpp"

namespace Tarbora {
    SceneNode::SceneNode(const ActorId &id, const std::string &name)
    {
        id_ = id;
        name_ = name;
        parent_ = nullptr;

        position_ = glm::vec3(0.f);
        rotation_ = glm::quat();
        scale_ = glm::vec3(1.f);
        global_scale_ = 1.f;
        origin_ = glm::vec3(0.f);

        radius_ = 0;
    }

    SceneNode::~SceneNode()
    {
        children_.clear();
    }

    void SceneNode::update(Scene *scene, float delta_time)
    {
        for (auto child : children_)
            child.second->update(scene, delta_time);
    }

    void SceneNode::drawChildren(Scene *scene, const glm::mat4 &parent_transform)
    {
        const glm::mat4 &transform = parent_transform * getLocalTransform();
        glm::mat4 deform = glm::scale(glm::mat4(1.f), scale_);

        draw(scene, transform * deform);

        for (auto child : children_)
        {
            if (child.second->isVisible(scene))
                child.second->drawChildren(scene, transform);
        }

        afterDraw(scene);
    }

    bool SceneNode::addChild(std::shared_ptr<SceneNode> child)
    {
        children_[child->getName()] = child;
        child->parent_ = this;
        float new_radius = child->getPosition().length() + child->getRadius();
        if (new_radius > radius_)
        {
            radius_ = new_radius;
        }
        return true;
    }

    std::shared_ptr<SceneNode> SceneNode::getChild(const std::string &name)
    {
        auto itr = children_.find(name);
        if (itr != children_.end())
        {
            return itr->second;
        }
        return std::shared_ptr<SceneNode>();
    }

    bool SceneNode::removeChild(const std::string &name)
    {
        auto child = children_.find(name);
        if (child != children_.end())
        {
            children_.erase(child);
            return true;
        }
        return false;
    }

    bool SceneNode::isVisible(Scene *scene)
    {
        UNUSED(scene);
        return true;
    }

    void SceneNode::setScale(const glm::vec3 &scale)
    {
        if (scale.x > 0.f && scale.y > 0.f && scale.z > 0.f)
        {
            origin_ *= scale/scale_;
            scale_ = scale;
        }
    }

    void SceneNode::setGlobalScale(float scale)
    {
        if (scale > 0.f)
        {
            origin_ *= scale/global_scale_;
            global_scale_ = scale;
        }
    }

    void SceneNode::setOrigin(const glm::vec3 &origin)
    {
        origin_ = origin * global_scale_ * scale_;
    }

    glm::vec3 SceneNode::getOrigin()
    {
        return origin_ / global_scale_ / scale_;
    }

    glm::mat4 SceneNode::getGlobalTransform()
    {
        if (parent_)
            return parent_->getGlobalTransform() * getLocalTransform();
        else
            return getLocalTransform();
    }

    glm::mat4 SceneNode::getLocalTransform()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.f), position_ + origin_);
        transform = transform * glm::mat4_cast(rotation_);
        transform = glm::translate(transform, -origin_);
        transform = glm::scale(transform, glm::vec3(global_scale_));
        return transform;
    }

    const glm::mat4 Camera::getView()
    {
        glm::mat4 matrix = getGlobalTransform();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::vec3 position = glm::vec3(matrix[3]);
        glm::mat4 view = glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    const glm::mat4 Camera::getViewAngle()
    {
        glm::mat4 matrix = getGlobalTransform();
        glm::vec3 front = glm::vec3(matrix[2]);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), front, glm::vec3(0.0f, 1.0f, 0.0f));
        return view;
    }

    const glm::vec3 Camera::getViewPosition()
    {
        glm::mat4 matrix = getGlobalTransform();
        glm::vec3 position = glm::vec3(matrix[3]);
        return position;
    }

    MaterialNode::MaterialNode(const ActorId &id, const std::string &name, const std::string &material) :
        SceneNode(id, name)
    {
        material_ = ResourcePtr<Material>("materials/" + material, "materials/missing.mat.lua");
        material_name_ = material;
    }

    void MaterialNode::draw(Scene *scene, const glm::mat4 &transform)
    {
        UNUSED(transform);
        scene->getRenderQueue()->pushMaterial(material_);
    }

    void MaterialNode::afterDraw(Scene *scene)
    {
        scene->getRenderQueue()->popMaterial();
    }

    MeshNode::MeshNode(const ActorId &id, const std::string &name, RenderPass render_pass, const std::string &mesh) :
        SceneNode(id, name), render_pass_(render_pass)
    {
        mesh_ = ResourcePtr<Mesh>("meshes/" + mesh, "meshes/cube.mesh");
        mesh_name_ = mesh;

        uv_map_ = glm::tvec2<unsigned short>(0);
        mesh_size_ = glm::vec3(0.f);
        texture_size_ = glm::vec3(0.f);
        color_primary_ = glm::tvec3<unsigned char>(255);
        color_secondary_ = glm::tvec3<unsigned char>(255);
        color_detail_ = glm::tvec3<unsigned char>(255);
        color_detail2_ = glm::tvec3<unsigned char>(255);
    }

    void MeshNode::draw(Scene *scene, const glm::mat4 &transform)
    {
        if (mesh_ != nullptr)
        {
            scene->getRenderQueue()->drawMesh(
                render_pass_,
                mesh_,
                transform,
                uv_map_,
                mesh_size_,
                texture_size_,
                color_primary_,
                color_secondary_,
                color_detail_,
                color_detail2_
            );
        }
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
