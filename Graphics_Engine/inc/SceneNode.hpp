#pragma once
#include "glm/glm.hpp"
#include <string>
#include <memory>
#include <vector>

#define INVALID_ID 0

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

    class SceneNodeProperties
    {
        friend class SceneNode;
    public:
        ActorId GetActorId() const { return m_actorId; }
        const char *GetName() const { return m_name.c_str(); }
        glm::mat4 const &GetToWorld() const { return m_ToWorld; }
        glm::mat4 const &GetFromWorld() const { return m_FromWorld; }
        void GetTransform(glm::mat4 *toWorld, glm::mat4 *fromWorld, glm::mat4 *scale) const
        {
            if (toWorld) *toWorld = m_ToWorld;
            if (fromWorld) *fromWorld = m_FromWorld;
            if (scale) *scale = m_Scale;
        }
        float GetRadius() const { return m_radius; }
        RenderPass GetRenderPass() const { return m_render_pass; }
    protected:
        ActorId m_actorId;
        std::string m_name;

        glm::mat4 m_FromWorld, m_ToWorld, m_Scale;
        float m_radius;
        RenderPass m_render_pass;
    };

    class SceneNode;
    typedef std::shared_ptr<SceneNode> SceneNodePtr;

    class SceneNode
    {
        friend class Scene;
        typedef std::vector<SceneNodePtr> SceneNodeList;
    public:
        SceneNode(ActorId actorId, std::string name, RenderPass render_pass, const glm::mat4 *to=nullptr, const glm::mat4 *from=nullptr)
        {
            m_Parent = nullptr;
            m_Props.m_actorId = actorId;
            m_Props.m_name = name;
            m_Props.m_render_pass = render_pass;
            SetTransform(to, from);
            SetRadius(0);
        }
        virtual ~SceneNode() {}

        virtual void Update(Scene *scene, float deltaTime);
        virtual void Draw(Scene *scene, glm::mat4 *parentTransform) { (void)(scene); (void)(parentTransform); }
        virtual void DrawChildren(Scene *scene, glm::mat4 *parentTransform);

        virtual bool AddChild(SceneNodePtr child);
        virtual SceneNodePtr GetChild(ActorId id);
        virtual bool RemoveChild(ActorId id);

        bool IsVisible(Scene *scene);

        void SetTransform(const glm::mat4 *to=nullptr, const glm::mat4 *from=nullptr);
        void SetRadius(float radius) { m_Props.m_radius = radius; }
        void SetPosition(const glm::vec3 &pos) { m_Props.m_ToWorld[3] = glm::vec4(pos, 1); }
        glm::vec3 GetPosition() { return glm::vec3(m_Props.m_ToWorld[3]); }
        // TODO: GetDirection

        virtual const SceneNodeProperties *Get() const { return &m_Props; }
    protected:
        SceneNodeList m_Children;
        SceneNode *m_Parent;
        SceneNodeProperties m_Props;
    };

    class RootNode : public SceneNode
    {
    public:
        RootNode();
        virtual bool AddChild(SceneNodePtr child) override;
        virtual void DrawChildren(Scene *scene, glm::mat4 *parentTransform) override;
        virtual bool IsVisible(Scene *scene) const { (void)(scene); return true; }
    };

    // class MeshNode : public SceneNode
    // {
    // public:
    //     MeshNode(ActorId actorId, std::string name, glm::mat4 *to);
    //     virtual void Draw(Scene *scene, glm::mat4 *parentTransform) override;
    // };
}
