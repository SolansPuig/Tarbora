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

        void AddChild(ActorId id, SceneNodePtr);
        SceneNodePtr GetChild(ActorId id);
        bool RemoveChild(ActorId id);
    protected:
        SceneNodePtr m_Root;
        SceneActorMap m_ActorMap;
        glm::mat4 m_Matrix;
    };
}
