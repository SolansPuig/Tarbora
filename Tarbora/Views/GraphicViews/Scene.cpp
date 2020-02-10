#include "Scene.hpp"
#include <utility>

namespace Tarbora {
    Scene::Scene(GraphicView *view) :
        m_View(view)
    {
        m_Root = std::shared_ptr<RootNode>(new RootNode());
        m_Projection = glm::perspective(glm::radians(45.0f), m_View->GetGraphicsEngine()->GetWindow()->GetRatio(), 0.1f, 100.0f);
    }

    void Scene::Update(float deltaTime)
    {
        if (m_Root)
        {
            m_Root->Update(this, deltaTime);
        }
    }

    void Scene::Draw()
    {
        m_View->GetGraphicsEngine()->GetRenderQueue()->SetProjectionMatrix(m_Projection);
        m_View->GetGraphicsEngine()->GetRenderQueue()->SetViewMatrix(GetCamera()->GetView());

        if (m_Root)
        {
            m_Root->DrawChildren(this, glm::mat4(1.0f));
            m_View->GetGraphicsEngine()->GetRenderQueue()->Draw();
        }
    }

    std::shared_ptr<Skybox> Scene::CreateSkybox(std::string material)
    {
        m_Skybox = std::shared_ptr<Skybox>(new Skybox(material));
        AddActor(m_Skybox);
        return m_Skybox;
    }

    std::shared_ptr<ActorModel> Scene::CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material)
    {
        std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, renderPass, model, material));
        AddActor(actor);
        return actor;
    }

    std::shared_ptr<ActorModel> Scene::CreateActorModel(ActorId id, std::string entity, std::string variant)
    {
        ResourcePtr<LuaScript> resource("entities/" + entity);
        if (resource != nullptr)
        {
            LuaTable modelComponent = resource->Get("components").Get("model");
            int renderPass = modelComponent.Get<int>("render_pass", 1);
            std::string model = modelComponent.Get<std::string>("model", "cube");
            std::string material = modelComponent.Get<std::string>("material", "white");

            std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, (RenderPass)renderPass, model, material));

            LuaTable animations = resource->Get("components").Get("animations", true);
            if (animations.Valid())
            {
                actor->Animate("idle", animations.Get<std::string>("file", entity + ".lua"));
            }

            AddActor(actor);
            return actor;
        }

        return std::shared_ptr<ActorModel>();
    }

    void Scene::AnimateActor(ActorId id, std::string animation)
    {
        std::shared_ptr<SceneNode> child = GetActor(id);
        std::static_pointer_cast<ActorModel>(child)->Animate(animation);
    }

    std::shared_ptr<Camera> Scene::CreateCamera(ActorId id)
    {
        std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera(id, "body"));
        AddActor(camera);
        return camera;
    }

    void Scene::SetCamera(ActorId id, std::string nodeName)
    {
        std::shared_ptr<SceneNode> actor = GetActor(id);
        if (actor)
        {
            std::shared_ptr<SceneNode> camera = actor->GetChild(nodeName);
            if (camera)
            {
                m_Camera = std::static_pointer_cast<Camera>(camera);
            }
        }
    }

    void Scene::AddActor(std::shared_ptr<SceneNode> actor)
    {
        ActorId id = actor->GetActorId();
        if (id != "")
            m_ActorMap[id] = actor;
        m_Root->AddChild(actor);
    }

    std::shared_ptr<SceneNode> Scene::GetActor(ActorId id)
    {
        auto itr = m_ActorMap.find(id);
        if (itr == m_ActorMap.end())
        {
            return std::shared_ptr<SceneNode>();
        }
        return itr->second;
    }

    bool Scene::RemoveActor(ActorId id)
    {
        if (id != "")
        {
            std::shared_ptr<SceneNode> child = GetActor(id);
            m_ActorMap.erase(id);
            return m_Root->RemoveChild(id);
        }
        return false;
    }
}
