#include "ActorModel.hpp"
#include "Scene.hpp"

namespace Tarbora {
    ActorModel::ActorModel(ActorId id, RenderPass renderPass, const std::string &model, const std::string &material)
        : MaterialNode(id, std::to_string(id), material)
    {
        ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
        // float scale = resource->Get<float>("scale");
        std::shared_ptr<MeshNode> mesh = CreateNode(id, renderPass, resource->Get("root"));
        // mesh->SetGlobalScale(glm::vec3(scale, scale, scale));

        AddChild(mesh);
    }

    std::shared_ptr<MeshNode> ActorModel::CreateNode(ActorId id, RenderPass renderPass, LuaTable table)
    {
        // Read all the parameters for the node
        std::string name = table.Get<std::string>("name");
        std::string shape = table.Get<std::string>("shape");

        // Create the node
        std::shared_ptr<MeshNode> node = std::shared_ptr<MeshNode>(new MeshNode(id, name, renderPass, shape));
        node->SetOrigin(table.Get<glm::vec3>("origin", true));
        node->Set("position", table.Get<glm::vec3>("position", true)/100.f);
        node->Set("rotation", table.Get<glm::vec3>("rotation", true));
        node->Set("uv_map", table.Get<glm::vec3>("uv_map", true));
        node->Set("color_primary", table.Get<glm::vec3>("color_primary", glm::vec3(255.f), true)/255.f);
        node->Set("color_secondary", table.Get<glm::vec3>("color_secondary", glm::vec3(255.f), true)/255.f);
        node->Set("color_detail1", table.Get<glm::vec3>("color_detail1", glm::vec3(255.f), true)/255.f);
        node->Set("color_detail2", table.Get<glm::vec3>("color_detail2", glm::vec3(255.f), true)/255.f);
        glm::vec3 scale = table.Get<glm::vec3>("size", true)/100.f;
        node->Set("scale", scale);
        node->Set("mesh_size", scale);
        glm::vec3 textureSize = table.Get<glm::vec3>("texture_size", true)/100.f;
        if (textureSize == glm::vec3(0.0f)) textureSize = scale;
        node->Set("texture_size", textureSize);

        // Create all its child nodes and add them as children to this
        for (auto n : table.Get("nodes", true))
        {
            std::shared_ptr<MeshNode> new_node = CreateNode(id, renderPass, n.second.GetAs<LuaTable>());
            node->AddChild(new_node);
        }

        // Create the child cameras, if any
        for (auto c : table.Get("cameras", true))
        {
            std::shared_ptr<Camera> newCamera = CreateCamera(id, c.second.GetAs<LuaTable>());
            node->AddChild(newCamera);
        }

        m_Nodes[name] = node;

        return node;
    }

    std::shared_ptr<Camera> ActorModel::CreateCamera(ActorId id, LuaTable table)
    {
        // Read all the parameters for the node
        std::string name = table.Get<std::string>("name");

        // Create the node
        std::shared_ptr<Camera> node = std::shared_ptr<Camera>(new Camera(id, name));
        node->SetOrigin(table.Get<glm::vec3>("origin", true));
        node->Set("position", table.Get<glm::vec3>("position", true)/100.f);
        node->Set("rotation", table.Get<glm::vec3>("rotation", true));

        m_Nodes[name] = node;

        return node;
    }

    void ActorModel::Update(Scene *scene, float deltaTime)
    {
        if (m_AnimationController)
        {
            m_AnimationController->Update(deltaTime);
        }

        MaterialNode::Update(scene, deltaTime);
    }

    std::shared_ptr<SceneNode> ActorModel::GetChild(const std::string &name)
    {
        auto itr = m_Nodes.find(name);
        if (itr != m_Nodes.end())
        {
            return itr->second;
        }
        return std::shared_ptr<SceneNode>();
    }

    void ActorModel::Animate(const std::string &animation_name, const std::string &animations_file)
    {
        if (!m_AnimationController)
        {
            if (animations_file == "")
            {
                LOG_ERR("Trying to animate an actor model with no Animation Controller defined and without an animations file");
                return;
            }
            m_AnimationController.emplace(this, animations_file);
        }

        m_AnimationController->SetAnimation(animation_name);
    }
}
