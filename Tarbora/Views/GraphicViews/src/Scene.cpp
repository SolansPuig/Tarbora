#include "../inc/Scene.hpp"
#include <utility>
#include "../../../Messages/BasicMessages.hpp"

namespace Tarbora {
    Scene::Scene(GraphicView *view) :
        m_View(view)
    {
        m_Root = std::shared_ptr<RootNode>(new RootNode());
        m_Camera = std::shared_ptr<Camera>(new Camera(MAIN_CAMERA_ID, "body"));
        AddChild(m_Camera);
        m_Camera->Rotate(glm::vec3(0.0f, 180.0f, 0.0f));
        m_Camera->Move(glm::vec3(0.0f, 0.0f, -5.0f));

        m_Projection = glm::perspective(glm::radians(45.0f), m_View->GetGraphicsEngine()->GetWindow()->GetRatio(), 0.1f, 100.0f);

        // EvtWindowResizeId = EventManager::Subscribe("WindowResize", [this](Event *e)
        // {
        //     (void)(e);
        //     m_Projection = glm::perspective(glm::radians(45.0f), m_View->graphicsEngine.window->GetRatio()->GetRatio(), 0.1f, 100.0f);
        // });
    }

    Scene::~Scene()
    {
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

    void Scene::CreateSkybox(std::string material)
    {
        m_Skybox = std::shared_ptr<Skybox>(new Skybox(material));
        AddChild(m_Skybox);
    }

    void Scene::CreateActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material)
    {
        std::shared_ptr<ActorModel> actor = std::shared_ptr<ActorModel>(new ActorModel(id, renderPass, model, material));
        AddChild(actor);
    }

    void Scene::CreateActorModel(ActorId id, std::string entity, std::string variant)
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
    }

    void Scene::AnimateActor(ActorId id, std::string animation)
    {
        SceneNodePtr child = GetChild(id);
        std::static_pointer_cast<ActorModel>(child)->Animate(animation);
    }

    void Scene::SetCamera(ActorId id, std::string nodeName)
    {
        SceneNodePtr actor = GetChild(id);
        if (actor)
        {
            SceneNodePtr camera = actor->GetChild(nodeName);
            if (camera)
            {
                m_Camera = std::static_pointer_cast<Camera>(camera);
            }
        }
    }

    void Scene::AddChild(SceneNodePtr child)
    {
        ActorId id = child->GetActorId();
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
