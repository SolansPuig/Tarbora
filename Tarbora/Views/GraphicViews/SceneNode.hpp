#pragma once
#include "GraphicView.hpp"

namespace Tarbora {
    class Scene;

    class SceneNode
    {
        friend class Scene;

    public:
        SceneNode(const ActorId &id, const std::string &name);
        ~SceneNode();

        virtual const std::string getNodeType() { return "SCENE"; }

        virtual void update(Scene *scene, float delta_time);
        virtual void drawChildren(Scene *scene, const glm::mat4 &parent_transform);
        virtual void draw(Scene *scene, const glm::mat4 &transform) { UNUSED(scene); UNUSED(transform); }
        virtual void afterDraw(Scene *scene) { UNUSED(scene); }

        bool addChild(std::shared_ptr<SceneNode> child);
        virtual std::shared_ptr<SceneNode> getChild(const std::string &name);
        SceneNode* getParent() { return parent_; }
        bool removeChild(const std::string &name);

        bool isVisible(Scene *scene);

        const ActorId& getActorId() const { return id_; }
        const std::string& getName() const { return name_; }

        void setPosition(const glm::vec3 &position) { position_ = position; }
        void setRotation(const glm::vec3 &rotation) { rotation_ = glm::quat(glm::radians(rotation)); }
        void setRotation(const glm::quat &rotation) { rotation_ = rotation; }
        void setScale(const glm::vec3 &scale);
        void setGlobalScale(float scale);
        void setOrigin(const glm::vec3 &origin);

        const glm::vec3& getPosition() { return position_; }
        glm::vec3 getRotation() { return glm::degrees(glm::eulerAngles(rotation_)); }
        const glm::quat& getRotationQuat() { return rotation_; }
        const glm::vec3& getScale() { return scale_; }
        float getGlobalScale() { return global_scale_; }
        glm::vec3 getOrigin();

        glm::mat4 getGlobalTransform();
        glm::mat4 getLocalTransform();

        void setRadius(float radius) { radius_ = radius; }
        float getRadius() const { return radius_; }

        bool size() { return children_.size(); }
        auto begin() { return children_.begin(); }
        auto end() { return children_.end(); }

    protected:
        ActorId id_;
        std::string name_;

        std::map<std::string, std::shared_ptr<SceneNode>> children_;
        SceneNode *parent_;

        glm::vec3 position_;
        glm::quat rotation_;
        glm::vec3 scale_;
        float global_scale_;
        glm::vec3 origin_;

        float radius_;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode() : SceneNode("", "Root") {}
        virtual const std::string getNodeType() { return "ROOT"; }
        virtual bool isVisible(Scene *scene) const { UNUSED(scene); return true; }
    };

    class Camera : public SceneNode
    {
    public:
        Camera(const ActorId &id, const std::string &name)
            : SceneNode(id, name) {}
        virtual const std::string getNodeType() { return "CAMERA"; }
        const glm::mat4 getView();
        const glm::mat4 getViewAngle();
        const glm::vec3 getViewPosition();
    private:
        glm::mat4 view_;
    };

    class MaterialNode : public SceneNode
    {
    public:
        MaterialNode(const ActorId &id, const std::string &name, const std::string &material);
        virtual const std::string getNodeType() { return "MATERIAL"; }
        virtual void draw(Scene *scene, const glm::mat4 &transform);
        virtual void afterDraw(Scene *scene);

        const std::string& getMaterial() { return material_name_; }
    protected:
        ResourcePtr<Material> material_;
        std::string material_name_;
    };

    class MeshNode : public SceneNode
    {
    public:
        MeshNode(const ActorId &id, const std::string &name, RenderPass render_pass, const std::string &mesh);
        virtual const std::string getNodeType() { return "MESH"; }
        virtual void draw(Scene *scene, const glm::mat4 &transform);

        void setUvMap(const glm::tvec2<unsigned short> &uv) { uv_map_ = uv; }
        void setMeshSize(const glm::vec3 &size) { mesh_size_ = size; }
        void setTextureSize(const glm::vec3 &size) { texture_size_ = size; }
        void setColorPrimary(const glm::tvec3<unsigned char> &color) { color_primary_ = color; }
        void setColorSecondary(const glm::tvec3<unsigned char> &color) { color_secondary_ = color; }
        void setColorDetail(const glm::tvec3<unsigned char> &color) { color_detail_ = color; }
        void setColorDetail2(const glm::tvec3<unsigned char> &color) { color_detail2_ = color; }

        const glm::tvec2<unsigned short>& getUvMap() { return uv_map_; }
        const glm::vec3& getMeshSize() { return mesh_size_; }
        const glm::vec3& getTextureSize() { return texture_size_; }
        const glm::tvec3<unsigned char>& getColorPrimary() { return color_primary_; }
        const glm::tvec3<unsigned char>& getColorSecondary() { return color_secondary_; }
        const glm::tvec3<unsigned char>& getColorDetail() { return color_detail_; }
        const glm::tvec3<unsigned char>& getColorDetail2() { return color_detail2_; }
        RenderPass getRenderPass() { return render_pass_; }
        const std::string& getShape() { return mesh_name_; }

    protected:
        RenderPass render_pass_;
        ResourcePtr<Mesh> mesh_;
        std::string mesh_name_;

        glm::tvec2<unsigned short> uv_map_;
        glm::vec3 mesh_size_;
        glm::vec3 texture_size_;
        glm::tvec3<unsigned char> color_primary_;
        glm::tvec3<unsigned char> color_secondary_;
        glm::tvec3<unsigned char> color_detail_;
        glm::tvec3<unsigned char> color_detail2_;
    };
}
