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

        void AddChild(SceneNodePtr);
        SceneNodePtr GetChild(ActorId id);
        bool RemoveChild(ActorId id);

        void SetCamera(CameraPtr camera) { m_Camera = camera; }
        const CameraPtr GetCamera() const { return m_Camera; }

    protected:
        SceneNodePtr m_Root;
        CameraPtr m_Camera;
        SceneActorMap m_ActorMap;
        glm::mat4 m_Projection;
        unsigned int EvtWindowResizeId, EvtActorMoveId, EvtActorRotateId;
    };
}
