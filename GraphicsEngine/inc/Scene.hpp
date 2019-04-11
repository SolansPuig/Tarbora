#pragma once
#include "SceneNode.hpp"
#include <map>
#include <memory>

namespace Tarbora {
    typedef std::map<ActorId, SceneNodePtr> SceneActorMap;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Update(float deltaTime);
        void Draw();

        void AddChild(SceneNodePtr child, RenderPass renderPass);
        SceneNodePtr GetChild(ActorId id);
        bool RemoveChild(ActorId id);

        MeshNodePtr CreateNode(ActorId id, json j, float pixelDensity, float textureSize);
        void CreateActor(ActorId id, RenderPass renderPass, std::string model, std::string shader, std::string texture);

        void SetCamera(CameraPtr camera) { m_Camera = camera; }
        const CameraPtr GetCamera() const { return m_Camera; }

    protected:
        std::shared_ptr<RootNode> m_Root;
        CameraPtr m_Camera;
        SceneActorMap m_ActorMap;
        glm::mat4 m_Projection;
        unsigned int EvtWindowResizeId, EvtActorMoveId, EvtCreateActorModelId;
    };
}
