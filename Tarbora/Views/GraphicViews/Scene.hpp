#pragma once
#include "SceneNode.hpp"
#include "ActorModel.hpp"
#include "Skybox.hpp"
#include "GraphicView.hpp"

namespace Tarbora {
    typedef std::map<ActorId, std::shared_ptr<SceneNode>> SceneActorMap;

    class Scene
    {
    public:
        Scene(GraphicView *view);

        void Update(float deltaTime);
        void Draw();

        std::shared_ptr<ActorModel> CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material);
        std::shared_ptr<ActorModel> CreateActorModel(ActorId id, std::string entity, std::string variant);

        void AnimateActor(ActorId id, std::string animation);

        std::shared_ptr<Skybox> CreateSkybox(std::string material);
        std::shared_ptr<Skybox> GetSkybox() { return m_Skybox; }

        void AddActor(std::shared_ptr<SceneNode> actor);
        std::shared_ptr<SceneNode> GetActor(ActorId id);
        bool RemoveActor(ActorId id);

        std::shared_ptr<Camera> CreateCamera(ActorId id);
        void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; }
        void SetCamera(ActorId id, std::string nodeName);
        std::shared_ptr<Camera> GetCamera() { return m_Camera; }

        inline std::shared_ptr<GraphicsEngine> GetGraphicsEngine() { return m_View->GetGraphicsEngine(); }
        inline std::shared_ptr<RenderQueue> GetRenderQueue() { return m_View->GetGraphicsEngine()->GetRenderQueue(); }

    protected:
        std::shared_ptr<RootNode> m_Root;
        std::shared_ptr<Skybox> m_Skybox;
        std::shared_ptr<Camera> m_Camera;

        SceneActorMap m_ActorMap;

        glm::mat4 m_Projection;

        GraphicView *m_View;
    };
}
