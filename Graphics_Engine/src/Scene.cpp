#include "Scene.hpp"
#include <utility>
#include "Logger.hpp"
#include "Events.hpp"
#include "Graphics_Engine.hpp"
#include <glm/gtx/string_cast.hpp>

namespace Tarbora {
    Scene::Scene()
    {
        m_Root.reset(new RootNode());
        m_Camera.reset(new Camera(MAIN_CAMERA_ID, "body"));
        AddChild(m_Camera, RenderPass::Actor);
        m_Projection = glm::perspective(glm::radians(45.0f), Graphics_Engine::GetWindow()->GetRatio(), 0.1f, 100.0f);

        CreateActor(15, "models/human.json", "shaders/model.shader.json", "textures/male.png");

        EventFn changeProjection = [this](Event* e)
        {
            (void)(e);
            m_Projection = glm::perspective(glm::radians(45.0f), Graphics_Engine::GetWindow()->GetRatio(), 0.1f, 100.0f);
        };

        EvtWindowResizeId = EventManager::Subscribe(EventType::WindowResize, changeProjection);

        EventFn onActorEvent = [this](Event* e)
        {
            ActorEvent *ev = static_cast<ActorEvent*>(e);
            auto itr = m_ActorMap.find(ev->actorId);
            if (itr != m_ActorMap.end())
            {
                if (!itr->second->OnActorEvent(ev))
                    LOG_WARN("Scene: Target %u has no child named %s to parse event of type %d", ev->actorId, ev->name.c_str(), e->GetType());
            } else
            {
                LOG_WARN("Scene: There's no node with id %u", ev->actorId);
            }
        };
        EvtActorMoveId = EventManager::Subscribe(EventType::ActorMove, onActorEvent);
        EvtActorRotateId = EventManager::Subscribe(EventType::ActorRotate, onActorEvent);
    }

    Scene::~Scene()
    {
        EventManager::Unsubscribe(EventType::WindowResize, EvtWindowResizeId);
        EventManager::Unsubscribe(EventType::ActorMove, EvtActorMoveId);
        EventManager::Unsubscribe(EventType::ActorRotate, EvtActorRotateId);
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

    MeshNodePtr Scene::CreateNode(ActorId id, json j)
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
        glm::vec3 position = glm::vec3((float)p[0]/40, (float)p[1]/40, (float)p[2]/40);
        glm::vec3 rotation = glm::vec3(r[0], r[1], r[2]);
        glm::vec3 size = glm::vec3((float)s[0]/40, (float)s[1]/40, (float)s[2]/40);
        glm::vec3 texSize = glm::vec3((float)s[0]/136, (float)s[1]/136, (float)s[2]/136);
        glm::vec2 uv = glm::vec2((float)u[0]/136, (float)u[1]/136);

        // Create the node
        MeshNodePtr node = MeshNodePtr(new MeshNode(id, name, shape));
        node->SetUV(texSize, uv);
        node->SetOrigin(origin);
        node->TranslateLocal(position);
        node->Scale(size);
        node->RotateLocal(rotation);

        // Create all its child nodes and add them as children to this
        for (auto itr = j["nodes"].begin(); itr != j["nodes"].end(); itr++) {
            MeshNodePtr new_node = CreateNode(id, *itr);
            node->AddChild(new_node);
        }

        return node;
    }

    void Scene::CreateActor(ActorId id, std::string model, std::string shader, std::string texture)
    {
        json j = GET_RESOURCE(JsonResource, model)->GetJson();
        std::shared_ptr<MaterialNode> mat = std::shared_ptr<MaterialNode>(new MaterialNode(id, "texture", shader, texture));
        mat->AddChild(CreateNode(id, j["root"]));
        AddChild(mat, RenderPass::Static);
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
