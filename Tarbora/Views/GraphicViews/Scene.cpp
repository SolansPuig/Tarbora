#include "Scene.hpp"
#include <utility>
#include "../../Messages/BasicMessages.hpp"

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
        GetMessageManager()->TriggerLocal("update_projection", Matrix(m_Projection));
        GetMessageManager()->TriggerLocal("update_view", Matrix(GetCamera()->GetView()));

        if (m_Root)
        {
            m_Root->DrawChildren(this, glm::mat4(1.0f));
            m_View->GetGraphicsEngine()->GetRenderQueue()->Draw();
        }
    }

    std::shared_ptr<Skybox> Scene::CreateSkybox(std::string material)
    {
        m_Skybox = std::shared_ptr<Skybox>(new Skybox(material));
        AddChild(m_Skybox);
        return m_Skybox;
    }

    std::shared_ptr<ActorModel> Scene::CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material)
    {
        std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, renderPass, model, material));
        AddChild(actor);
        return actor;
    }

    std::shared_ptr<ActorModel> Scene::CreateActorModel(ActorId id, std::string entity, std::string variant)
    {
        JsonPtr resource = GET_RESOURCE(Json, "entities/" + entity);

        raw_json components;
        resource->Get("components", &components, {true, true});
        resource->PushErrName("components");

        raw_json modelJson;
        resource->Get(components, "model", &modelJson, {true, true});
        resource->PushErrName("model");

        int renderPass = 1;
        std::string model = entity + ".json";
        std::string material = entity + ".png";

        resource->Get(modelJson, "renderPass", &renderPass, {true});
        resource->Get(modelJson, "model", &model, {true});
        resource->Get(modelJson, "material", &material, {true});

        resource->PopErrName();

        std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, (RenderPass)renderPass, model, material));

        raw_json animationsJson;
        resource->Get(components, "animations", &animationsJson, {true, true});
        resource->PushErrName("animations");

        if (!animationsJson.empty())
        {
            std::string file = entity + ".json";;
            resource->Get(animationsJson, "file", &file, {true});
            actor->Animate("idle", file);
        }

        resource->PopErrName();
        resource->PopErrName();

        AddChild(actor);
        return actor;
    }

    void Scene::AnimateActor(ActorId id, std::string animation)
    {
        std::shared_ptr<SceneNode> child = GetChild(id);
        std::static_pointer_cast<ActorModel>(child)->Animate(animation);
    }

    std::shared_ptr<Camera> Scene::CreateCamera(ActorId id)
    {
        std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera(id, "body"));
        AddChild(camera);
        return camera;
    }

    void Scene::SetCamera(ActorId id, std::string nodeName)
    {
        std::shared_ptr<SceneNode> actor = GetChild(id);
        if (actor)
        {
            std::shared_ptr<SceneNode> camera = actor->GetChild(nodeName);
            if (camera)
            {
                m_Camera = std::static_pointer_cast<Camera>(camera);
            }
        }
    }

    void Scene::AddChild(std::shared_ptr<SceneNode> child)
    {
        ActorId id = child->GetActorId();
        if (id != INVALID_ID)
        {
            m_ActorMap[id] = child;
        }
        m_Root->AddChild(child);
    }

    std::shared_ptr<SceneNode> Scene::GetChild(ActorId id)
    {
        auto itr = m_ActorMap.find(id);
        if (itr == m_ActorMap.end())
        {
            return std::shared_ptr<SceneNode>();
        }
        return itr->second;
    }

    bool Scene::RemoveChild(ActorId id)
    {
        if (id == INVALID_ID)
        {
            return false;
        }
        std::shared_ptr<SceneNode> child = GetChild(id);
        m_ActorMap.erase(id);
        return m_Root->RemoveChild(id);
    }
}
