#pragma once
#include "../../GraphicsEngine/GraphicsEngine.hpp"

namespace Tarbora {
    class Scene;

    class SceneNode;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

    class SceneNode
    {
        friend class Scene;
        typedef std::map<std::string, SceneNodePtr> SceneNodeMap;
    public:
        SceneNode(ActorId actorId, std::string name);
        virtual ~SceneNode() {}

        virtual void Update(Scene *scene, float deltaTime);
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform) { (void)(scene); (void)(parentTransform); }
        virtual void DrawChildren(Scene *scene, const glm::mat4 &parentTransform);
        virtual void AfterDraw(Scene *scene) { (void)(scene); }

        virtual bool AddChild(SceneNodePtr child);
        virtual SceneNodePtr GetChild(ActorId id);
        virtual SceneNodePtr GetChild(std::string name);
        virtual bool RemoveChild(ActorId id);
        virtual bool RemoveChild(std::string name);

        // bool OnActorEvent(ActorEvent *e);

        bool IsVisible(Scene *scene);

        // TODO: GetDirection

        ActorId GetActorId() const { return m_ActorId; }
        const char *GetName() const { return m_Name.c_str(); }

        void SetPosition(const glm::vec3 &position);
        void Move(const glm::vec3 &vector);
        void MoveTo(const glm::vec3 &position, float timeToComplete);
        const glm::vec3 &GetPosition() { return m_Position; }

        void SetRotation(const glm::vec3 &rotation);
        void Rotate(const glm::vec3 &angles);
        void RotateTo(const glm::vec3 &rotation, float timeToComplete);
        const glm::vec3 &GetRotation() { return m_Rotation; }

        void SetRotationMatrix(const glm::mat3 &rotation);

        void SetScale(const glm::vec3 &scale);
        void SetGlobalScale(const glm::vec3 &scale);
        void Scale(const glm::vec3 &scale);
        void ScaleTo(const glm::vec3 &scale, float timeToComplete);
        const glm::vec3 &GetScale() { return m_Scale; }

        void SetShear(const glm::vec3 &shearA, const glm::vec3 &shearB);
        void Shear(const glm::vec3 &shearA, const glm::vec3 &shearB);

        void SetOrigin(const glm::vec3 &origin);

        void SetTransform(glm::mat4 *matrix=nullptr);
        const glm::mat4 GetGlobalTransform();
        const glm::mat4 &GetLocalTransform() const { return m_LocalMatrix; }

        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }

        void SetColorPrimary(glm::vec3 color) { m_ColorPrimary = color; }
        void SetColorSecondary(glm::vec3 color) { m_ColorSecondary = color; }
        void SetColorDetail(glm::vec3 color) { m_ColorDetail = color; }
        void SetColorDetail2(glm::vec3 color) { m_ColorDetail2 = color; }

    protected:
        bool InterpolatePosition(float fraction);
        bool InterpolateRotation(float fraction);
        bool InterpolateScale(float fraction);

        SceneNodeMap m_Children;
        SceneNode *m_Parent;

        ActorId m_ActorId;
        std::string m_Name;

        glm::mat4 m_LocalMatrix;
        glm::mat4 m_DeformationMatrix;

        glm::vec3 m_OldPosition;
        glm::vec3 m_Position;
        glm::vec3 m_TargetPosition;
        float m_PositionCounter;
        float m_PositionTime;
        glm::vec3 m_Rotation;
        glm::vec3 m_OldRotation;
        glm::vec3 m_TargetRotation;
        float m_RotationCounter;
        float m_RotationTime;
        glm::vec3 m_OldScale;
        glm::vec3 m_Scale;
        glm::vec3 m_TargetScale;
        glm::vec3 m_ShearA;
        glm::vec3 m_ShearB;
        float m_ScaleCounter;
        float m_ScaleTime;

        glm::vec3 m_ColorPrimary;
        glm::vec3 m_ColorSecondary;
        glm::vec3 m_ColorDetail;
        glm::vec3 m_ColorDetail2;

        glm::vec3 m_Origin;

        float m_Radius;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode();
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return true; }
    };

    class Camera : public SceneNode
    {
    public:
        Camera(ActorId actorId, std::string name);
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
        MaterialNode(ActorId actorId, std::string name, std::string material);
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
        virtual void AfterDraw(Scene *scene);
    protected:
        ResourcePtr<Material> m_Material;
    };

    class MeshNode : public SceneNode
    {
    public:
        MeshNode(ActorId actorId, std::string name, RenderPass renderPass, std::string mesh);
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
        void SetUV(glm::vec3 &size, glm::vec2 &uv);

    protected:
        RenderPass m_RenderPass;
        ResourcePtr<Mesh> m_Mesh;
        glm::vec2 m_Uv;
        glm::vec3 m_TextureSize;
    };
}
