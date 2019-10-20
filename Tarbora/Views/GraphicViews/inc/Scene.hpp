#pragma once
#include "SceneNode.hpp"
#include "ActorModel.hpp"
#include "Skybox.hpp"
#include "GraphicView.hpp"
#include "../../GraphicsEngine/GraphicsEngine.hpp"

namespace Tarbora {
    typedef std::map<ActorId, SceneNodePtr> SceneActorMap;

    class Scene
    {
    public:
        Scene(GraphicView *view);
        ~Scene();

        void Update(float deltaTime);
        void Draw();

        void CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material);
        void CreateActorModel(ActorId id, std::string entity, std::string variant);

        void AnimateActor(ActorId id, std::string animation);

        void CreateSkybox(std::string material);
        std::shared_ptr<Skybox> GetSkybox() { return m_Skybox; }

        void AddChild(SceneNodePtr child);
        SceneNodePtr GetChild(ActorId id);
        bool RemoveChild(ActorId id);

        void SetCamera(CameraPtr camera) { m_Camera = camera; }
        void SetCamera(ActorId id, std::string nodeName);
        CameraPtr GetCamera() { return m_Camera; }

        inline std::shared_ptr<GraphicsEngine> GetGraphicsEngine() { return m_View->GetGraphicsEngine(); }
        inline std::shared_ptr<MessageManager> GetMessageManager() { return m_View->GetMessageManager(); }
        inline std::shared_ptr<RenderQueue> GetRenderQueue() { return m_View->GetGraphicsEngine()->GetRenderQueue(); }

    protected:
        std::shared_ptr<RootNode> m_Root;
        std::shared_ptr<Skybox> m_Skybox;
        CameraPtr m_Camera;
        SceneActorMap m_ActorMap;

        glm::mat4 m_Projection;

        GraphicView *m_View;

    };
}
