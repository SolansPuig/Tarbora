#pragma once
#include "SceneNode.hpp"
#include "Logger.hpp"
#include "Graphics_Engine.hpp"

namespace Tarbora {
    void SceneNode::Update(Scene *scene, float deltaTime)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
            (*itr)->Update(scene, deltaTime);
    }

    void SceneNode::DrawChildren(Scene *scene, glm::mat4 *parentTransform)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
        {
            glm::mat4 newTransform = *parentTransform * m_Props.m_ToWorld;
            if ((*itr)->IsVisible(scene))
            {
                // TODO: Check alpha
                (*itr)->Draw(scene, &newTransform);
                (*itr)->DrawChildren(scene, &newTransform);
            }
        }
    }

    bool SceneNode::AddChild(SceneNodePtr child)
    {
        m_Children.push_back(child);
        child->m_Parent = this;
        float new_radius = child->GetPosition().length() + child->Get()->GetRadius();
        if (new_radius > m_Props.m_radius)
            m_Props.m_radius = new_radius;
        return true;
    }

    SceneNodePtr SceneNode::GetChild(ActorId id)
    {
        (void)(id);
        return SceneNodePtr();
    }

    bool SceneNode::RemoveChild(ActorId id)
    {
        for (auto itr = m_Children.begin(); itr != m_Children.end(); itr++)
    	{
    		if (id == (*itr)->Get()->GetActorId())
    		{
    			m_Children.erase(itr);
    			return true;
    		}
    	}
        return false;
    }

    bool SceneNode::IsVisible(Scene *scene)
    {
        (void)(scene);
        return true;
    }

    void SceneNode::SetTransform(const glm::mat4 *to, const glm::mat4 *from)
    {
        if (!to)
            m_Props.m_ToWorld = glm::mat4();
        else
            m_Props.m_ToWorld = *to;
        if (!from)
            m_Props.m_FromWorld = glm::inverse(m_Props.GetToWorld());
        else
            m_Props.m_FromWorld = *from;
    }

    RootNode::RootNode() : SceneNode(INVALID_ID, "Root", RenderPass::Zero)
    {
        m_Children.reserve(RenderPass::Last);
        SceneNodePtr staticGroup(new SceneNode(INVALID_ID, "StaticGroup", RenderPass::Static));
        m_Children.push_back(staticGroup);
        SceneNodePtr actorGroup(new SceneNode(INVALID_ID, "ActorGroup", RenderPass::Actor));
        m_Children.push_back(actorGroup);
        SceneNodePtr skyGroup(new SceneNode(INVALID_ID, "SkyGroup", RenderPass::Sky));
        m_Children.push_back(skyGroup);
        SceneNodePtr invisibleGroup(new SceneNode(INVALID_ID, "InvisibleGroup", RenderPass::NotRendered));
        m_Children.push_back(invisibleGroup);
    }

    void RootNode::DrawChildren(Scene *scene, glm::mat4 *parentTransform)
    {
        for (int pass = RenderPass::Zero; pass < RenderPass::Last; pass++)
        {
            switch (pass)
            {
                case RenderPass::Static:
                case RenderPass::Actor:
                    m_Children[pass]->DrawChildren(scene, parentTransform);
                    break;
                case RenderPass::Sky:
                    m_Children[pass]->DrawChildren(scene, parentTransform);
                    break;
            }
        }
    }

    bool RootNode::AddChild(SceneNodePtr child)
    {
        RenderPass pass = child->Get()->GetRenderPass();
        if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
        {
            LOG_ERR("RootNode: Unexisting render pass.");
            return false;
        }

        return m_Children[pass]->AddChild(child);
    }

    // MeshNode::MeshNode(ActorId actorId, RenderPass renderPass std::string name, glm::mat4 *to) : SceneNode(actorId, name, renderPass, to) {};
    //
    // MeshNode::Draw(Scene *scene, glm::mat4 parentTransform)
    // {
    //     Graphics_Engine::GetShader()->SetMat4("model", *parentTransform * m_ToWorld);
    // }
}
