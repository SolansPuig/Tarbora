#pragma once
#include "../../GraphicsEngine/GraphicsEngine.hpp"
#include "NodeProperty.hpp"

namespace Tarbora {
    class Scene;

    class SceneNode;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

    class SceneNode
    {
        friend class Scene;
        typedef std::map<std::string, SceneNodePtr> SceneNodeMap;
        typedef std::unordered_map<std::string, PropertyPtr> PropertyMap;
    public:
        SceneNode(ActorId actorId, const std::string &name);
        virtual ~SceneNode() {}

        virtual void Update(Scene *scene, float deltaTime);
        virtual void Draw(Scene *scene, const glm::mat4 &parentTransform) { (void)(scene); (void)(parentTransform); }
        virtual void DrawChildren(Scene *scene, const glm::mat4 &parentTransform);
        virtual void AfterDraw(Scene *scene) { (void)(scene); }

        virtual bool AddChild(SceneNodePtr child);
        virtual SceneNodePtr GetChild(ActorId id);
        virtual SceneNodePtr GetChild(const std::string &name);
        virtual bool RemoveChild(ActorId id);
        virtual bool RemoveChild(const std::string &name);

        bool IsVisible(Scene *scene);

        ActorId GetActorId() const { return m_ActorId; }
        const char *GetName() const { return m_Name.c_str(); }

        void SetPosition(const glm::vec3 &position);
        void SetRotationMatrix(const glm::mat3 &rotation);

        void SetTransform(const glm::mat4 &matrix);
        const glm::mat4 GetGlobalTransform();
        const glm::mat4 &GetLocalTransform() const { return m_Transformation; }

        void Set(const std::string &name, const glm::vec3 &value);
        void Add(const std::string &name, const glm::vec3 &value);
        void InterpolateTo(const std::string &name, const glm::vec3 &value, float timeToComplete);
        const glm::vec3 &Get(const std::string &name);

        void SetOrigin(const glm::vec3 &origin) { m_Origin = origin; }

        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }

    protected:
        SceneNodeMap m_Children;
        SceneNode *m_Parent;

        PropertyMap m_Properties;

        ActorId m_ActorId;
        std::string m_Name;

        glm::mat4 m_Transformation;
        glm::mat4 m_Deformation;

        glm::vec3 m_Origin;

        float m_Radius;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode()
            : SceneNode(INVALID_ID, "Root") {}
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return true; }
    };

    class Camera : public SceneNode
    {
    public:
        Camera(ActorId actorId, const std::string &name)
            : SceneNode(actorId, name) {}
        const glm::mat4 GetView();
        const glm::mat4 GetViewAngle();
        const glm::vec3 GetViewPosition();
    private:
        glm::mat4 m_View;
    };
    typedef std::shared_ptr<Camera> CameraPtr;

    class MaterialNode : public SceneNode
    {
    public:
        MaterialNode(ActorId actorId, const std::string &name, const std::string &material);
        virtual void Draw(Scene *scene, const glm::mat4 &parentTransform);
        virtual void AfterDraw(Scene *scene);
    protected:
        ResourcePtr<Material> m_Material;
    };

    class MeshNode : public SceneNode
    {
    public:
        MeshNode(ActorId actorId, const std::string &name, RenderPass renderPass, const std::string &mesh);
        virtual void Draw(Scene *scene, const glm::mat4 &parentTransform);

        void SetUV(const glm::vec3 &size, const glm::vec2 &uv);

    protected:
        RenderPass m_RenderPass;
        ResourcePtr<Mesh> m_Mesh;

        glm::vec2 m_Uv;
        glm::vec3 m_TextureSize;
    };
}
