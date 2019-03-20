#include "Scene.hpp"
#include <utility>

namespace Tarbora {
    Scene::Scene()
    {
        if (m_Root)
        {
            m_Root.reset(new RootNode());
            m_Matrix = glm::mat4();
        }
    }

    Scene::~Scene() {}

    void Scene::Update(float deltaTime)
    {
        if (m_Root)
        {
            m_Root->Update(this, deltaTime);
        }
    }

    void Scene::Draw()
    {
        if (m_Root)
        {
            m_Root->DrawChildren(this, &m_Matrix);
        }
    }

    void Scene::AddChild(ActorId id, SceneNodePtr child)
    {
        if (id != INVALID_ID)
            m_ActorMap[id] = child;
        m_Root->AddChild(child);
    }

    SceneNodePtr Scene::GetChild(ActorId id)
    {
        auto itr = m_ActorMap.find(id);
        if (itr == m_ActorMap.end())
        {
            return SceneNodePtr();
        }
        return itr->second;
    }

    bool Scene::RemoveChild(ActorId id)
    {
        if (id == INVALID_ID)
            return false;
        SceneNodePtr child = GetChild(id);
        m_ActorMap.erase(id);
        return m_Root->RemoveChild(id);
    }
}
