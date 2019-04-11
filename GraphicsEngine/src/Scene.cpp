#include "Scene.hpp"
#include <utility>
#include "Logger.hpp"
#include "Events.hpp"
#include "GraphicsEngine.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {
    Scene::Scene()
    {
        m_Root.reset(new RootNode());
        m_Camera.reset(new Camera(MAIN_CAMERA_ID, "body"));
        glm::vec3 translate = glm::vec3(0.0f, 0.0f, -5.0f);
        glm::vec3 rotate = glm::vec3(0.0f, 180.0f, 0.0f);
        m_Camera->RotateLocal(rotate);
        m_Camera->TranslateLocal(translate);
        AddChild(m_Camera, RenderPass::Actor);
        m_Projection = glm::perspective(glm::radians(45.0f), GraphicsEngine::GetWindow()->GetRatio(), 0.1f, 100.0f);

        EvtWindowResizeId = EventManager::Subscribe(EventType::WindowResize, [this](Event *e)
        {
            (void)(e);
            m_Projection = glm::perspective(glm::radians(45.0f), GraphicsEngine::GetWindow()->GetRatio(), 0.1f, 100.0f);
        });

        EvtCreateActorModelId = EventManager::Subscribe(EventType::CreateActorModel, [this](Event *e)
        {
            CreateActorModelEvent *ev = static_cast<CreateActorModelEvent*>(e);
            CreateActor(ev->actorId, static_cast<RenderPass>(ev->renderPass), ev->model, ev->shader, ev->texture);
        });

        EventFn onActorEvent = [this](Event* e)
        {
            ActorEvent *ev = static_cast<ActorEvent*>(e);
            auto itr = m_ActorMap.find(ev->actorId);
            if (itr != m_ActorMap.end())
            {
                if (!itr->second->OnActorEvent(ev))
                    LOG_WARN("Scene: Target %u cannot parse event of type %d", ev->actorId, e->GetType());
            } else
            {
                LOG_WARN("Scene: There's no node with id %u", ev->actorId);
            }
        };
        EvtActorMoveId = EventManager::Subscribe(EventType::ActorMove, onActorEvent);
    }

    Scene::~Scene()
    {
        EventManager::Unsubscribe(EventType::WindowResize, EvtWindowResizeId);
        EventManager::Unsubscribe(EventType::ActorMove, EvtActorMoveId);
        EventManager::Unsubscribe(EventType::CreateActorModel, EvtCreateActorModelId);
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
        if (m_Root)
        {
            m_Root->DrawChildren(this, m_Projection);
        }
    }

    MeshNodePtr Scene::CreateNode(ActorId id, json j, float pixelDensity, float textureSize)
    {
        // Read all the parameters for the node
        std::string name = j["name"];
        std::string shape = j["shape"];

        json o = j["origin"];
        json p = j["position"];
        json r = j["rotation"];
        json s = j["size"];
        json u = j["uv"];
        glm::vec3 origin = glm::vec3(o[0], o[1], o[2]);
        glm::vec3 position = glm::vec3((float)p[0]/pixelDensity, (float)p[1]/pixelDensity, (float)p[2]/pixelDensity);
        glm::vec3 rotation = glm::vec3(r[0], r[1], r[2]);
        glm::vec3 size = glm::vec3((float)s[0]/pixelDensity, (float)s[1]/pixelDensity, (float)s[2]/pixelDensity);
        glm::vec3 texSize = glm::vec3((float)s[0]/textureSize, (float)s[1]/textureSize, (float)s[2]/textureSize);
        glm::vec2 uv = glm::vec2((float)u[0]/textureSize, (float)u[1]/textureSize);

        // Create the node
        MeshNodePtr node = MeshNodePtr(new MeshNode(id, name, shape));
        node->SetUV(texSize, uv);
        node->SetOrigin(origin);
        node->TranslateLocal(position);
        node->Scale(size);
        node->RotateLocal(rotation);

        // Create all its child nodes and add them as children to this
        for (auto itr = j["nodes"].begin(); itr != j["nodes"].end(); itr++) {
            MeshNodePtr new_node = CreateNode(id, *itr, pixelDensity, textureSize);
            node->AddChild(new_node);
        }

        return node;
    }

    void Scene::CreateActor(ActorId id, RenderPass renderPass, std::string model, std::string shader, std::string texture)
    {
        json j = GET_RESOURCE(JsonResource, model)->GetJson();
        std::shared_ptr<MaterialNode> mat = std::shared_ptr<MaterialNode>(new MaterialNode(id, std::to_string(id), shader, texture));
        mat->AddChild(CreateNode(id, j["root"], j["pixel_density"], j["texture_size"]));
        AddChild(mat, renderPass);
    }

    void Scene::AddChild(SceneNodePtr child, RenderPass renderPass)
    {
        ActorId id = child->GetActorId();
        if (id != INVALID_ID)
            m_ActorMap[id] = child;
        m_Root->AddChild(child, renderPass);
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
