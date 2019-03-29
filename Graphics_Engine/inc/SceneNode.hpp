#pragma once
#include "glm/glm.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Resource.hpp"
#include "Events.hpp"

#define INVALID_ID 0
#define MAIN_CAMERA_ID 1
#define SKY_ID 10

namespace Tarbora {
    class Scene;

    // Temporal
    typedef unsigned long ActorId;

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
        typedef std::vector<SceneNodePtr> SceneNodeList;
    public:
        SceneNode(ActorId actorId, std::string name, RenderPass render_pass, const glm::mat4 *matrix=nullptr);
        virtual ~SceneNode() {}

        virtual void Update(Scene *scene, float deltaTime);
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform) { (void)(scene); (void)(parentTransform); }
        virtual void DrawChildren(Scene *scene, glm::mat4 &parentTransform);

        virtual bool AddChild(SceneNodePtr child);
        virtual SceneNodePtr GetChild(ActorId id);
        virtual bool RemoveChild(ActorId id);

        bool OnActorEvent(ActorEvent *e);

        bool IsVisible(Scene *scene);

        // TODO: GetDirection

        ActorId GetActorId() const { return m_ActorId; }
        const char *GetName() const { return m_Name.c_str(); }

        void SetTransform(const glm::mat4 *matrix=nullptr);
        glm::mat4 const GetGlobalMatrix();
        glm::mat4 const &GetLocalMatrix() const { return m_LocalMatrix; }
        void SetPosition(const glm::vec3 &pos);
        glm::vec3 const GetPosition();
        void RotateGlobal(glm::vec3 &rotation);
        void RotateLocal(glm::vec3 &rotation);
        void RotateAround(float angle, glm::vec3 &dir);
        void MoveGlobal(glm::vec3 &movement);
        void MoveLocal(glm::vec3 &movement);
        void MoveTo(glm::vec3 &dir);


        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }
        RenderPass GetRenderPass() const { return m_RenderPass; }

    protected:
        SceneNodeList m_Children;
        SceneNode *m_Parent;

        ActorId m_ActorId;
        std::string m_Name;
        glm::mat4 m_LocalMatrix;
        float m_Radius;
        RenderPass m_RenderPass;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode();
        virtual bool AddChild(SceneNodePtr child) override;
        virtual void DrawChildren(Scene *scene, glm::mat4 &parentTransform) override;
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return true; }
    };

    class Skybox : public SceneNode
    {
    public:
        Skybox(std::string shader, std::string texture="");
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return m_Active; }
        void SetActive(bool active) { m_Active = active; }
    private:
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<MeshResource> m_Mesh;
        bool m_Active;
    };

    class Camera : public SceneNode
    {
    public:
        Camera(ActorId actorId, std::string name, glm::mat4 *matrix=nullptr);
        const glm::mat4 GetView();
        const glm::mat4 GetViewAngle();
    private:
        glm::mat4 m_View;
    };
    typedef std::shared_ptr<Camera> CameraPtr;

    class MeshNode : public SceneNode
    {
    public:
        MeshNode(ActorId actorId, std::string name, RenderPass renderPass, glm::mat4 *matrix, std::string mesh, std::string shader, std::string texture="");
        virtual void Draw(Scene *scene, glm::mat4 &parentTransform);
    protected:
        std::shared_ptr<Texture> m_Texture;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<MeshResource> m_Mesh;
    };
}
