#pragma once
#include "SceneNode.hpp"
#include "ActorModel.hpp"
#include "Skybox.hpp"
#include "GraphicView.hpp"

namespace Tarbora {
    typedef std::map<ActorId, SceneNodePtr> SceneActorMap;

    class Scene
    {
    public:
        Scene(GraphicView *app);
        ~Scene();

        void Update(float deltaTime);
        void Draw();

        void CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string shader, std::string texture);
        void CreateActorModel(ActorId id, std::string entity, std::string variant);

        void AnimateActor(ActorId id, std::string animation);

        void CreateSkybox(std::string shader, std::string texture);
        std::shared_ptr<Skybox> GetSkybox() const { return m_Skybox; }

        void AddChild(SceneNodePtr child, RenderPass renderPass);
        SceneNodePtr GetChild(ActorId id);
        bool RemoveChild(ActorId id);

        void SetCamera(CameraPtr camera) { m_Camera = camera; }
        const CameraPtr GetCamera() const { return m_Camera; }

        std::shared_ptr<GraphicsEngineImpl> GraphicsEngine() { return app->GraphicsEngine(); }

    protected:
        std::shared_ptr<RootNode> m_Root;
        std::shared_ptr<Skybox> m_Skybox;
        CameraPtr m_Camera;
        SceneActorMap m_ActorMap;

        glm::mat4 m_Projection;

        GraphicView *app;
    };
}
