#pragma once
#include "../../GraphicsEngine/inc/GraphicsEngine.hpp"

namespace Tarbora {
    class Scene;

    enum RenderPass
    {
        Zero,
        Static = Zero,
        Actor,
        Sky,
        NotRendered,
        Last
    };

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
        virtual void DrawChildren(Scene *scene, glm::mat4 &parentTransform);

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
        glm::vec3 const &GetPosition() { return m_Position; }

        void SetRotation(const glm::vec3 &rotation);
        void Rotate(const glm::vec3 &angles);
        void RotateTo(const glm::vec3 &rotation, float timeToComplete);
        glm::vec3 const &GetRotation() { return m_Rotation; }

        void SetRotationMatrix(const glm::mat3 &rotation);

        void SetScale(const glm::vec3 &scale);
        void Scale(const glm::vec3 &scale);
        void ScaleTo(const glm::vec3 &scale, float timeToComplete);
        glm::vec3 const &GetScale() { return m_Scale; }

        void SetOrigin(const glm::vec3 &origin);

        void SetTransform(glm::mat4 *matrix=nullptr);
        glm::mat4 const GetGlobalTransform();
        glm::mat4 const &GetLocalTransform() const { return m_LocalMatrix; }

        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }

    protected:
        bool InterpolatePosition(float fraction);
        bool InterpolateRotation(float fraction);
        bool InterpolateScale(float fraction);

        SceneNodeMap m_Children;
        SceneNode *m_Parent;

        ActorId m_ActorId;
        std::string m_Name;

        glm::mat4 m_LocalMatrix;

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
        float m_ScaleCounter;
        float m_ScaleTime;

        glm::vec3 m_Origin;

        float m_Radius;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode();
        virtual bool AddChild(SceneNodePtr child, RenderPass renderPass);
        virtual bool RemoveChild(ActorId id) override;
        virtual void DrawChildren(Scene *scene, glm::mat4 &parentTransform) override;
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return true; }
    };

    class Camera : public SceneNode
    {
    public:
        Camera(ActorId actorId, std::string name);
        const glm::mat4 GetView();
        const glm::mat4 GetViewAngle();
    private:
        glm::mat4 m_View;
    };
    typedef std::shared_ptr<Camera> CameraPtr;

    class MaterialNode : public SceneNode
    {
    public:
        MaterialNode(ActorId actorId, std::string name, std::string shader, std::string texture="");
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
    protected:
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<Shader> m_Shader;

        glm::vec3 m_InterpolatedPosition;
        glm::vec3 m_InterpolatedRotation;
        float m_InterpolatedScale;
        float m_NextAnimationFrame;
    };

    class MeshNode : public SceneNode
    {
    public:
        MeshNode(ActorId actorId, std::string name, std::string mesh);
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
        void SetUV(glm::vec3 &size, glm::vec2 &uv);

    protected:
        std::shared_ptr<Mesh> m_Mesh;
        glm::vec2 m_Uv;
        glm::vec3 m_TexSize;
    };
}
