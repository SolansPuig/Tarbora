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

    glm::vec3 ReadValue(LuaTable &table, const std::string &name, float scale=1.f, glm::vec3 def=glm::vec3(0.f))
    {
        glm::vec3 vec;
        for (int i = 0; i < 3; i++)
            vec[i] = table.Get(name, true).Get<float>(i+1, def[i], true)/scale;
        return vec;
    }

    std::shared_ptr<MeshNode> ActorModel::CreateNode(ActorId id, RenderPass renderPass, LuaTable table)
    {
        // Read all the parameters for the node
        std::string name = table.Get<std::string>("name");
        std::string shape = table.Get<std::string>("shape");

        // Create the node
        std::shared_ptr<MeshNode> node = std::shared_ptr<MeshNode>(new MeshNode(id, name, renderPass, shape));
        node->SetOrigin(ReadValue(table, "origin"));
        node->Set("position", ReadValue(table, "position", 100.f));
        node->Set("rotation", ReadValue(table, "rotation"));
        node->Set("uv_map", ReadValue(table, "uv_map"));
        node->Set("color_primary", ReadValue(table, "color_primary", 255.f, glm::vec3(255.f)));
        node->Set("color_secondary", ReadValue(table, "color_secondary", 255.f, glm::vec3(255.f)));
        node->Set("color_detail1", ReadValue(table, "color_detail1", 255.f, glm::vec3(255.f)));
        node->Set("color_detail2", ReadValue(table, "color_detail2", 255.f, glm::vec3(255.f)));
        glm::vec3 scale = ReadValue(table, "size", 100.f);
        node->Set("scale", scale);
        node->Set("mesh_size", scale);
        glm::vec3 textureSize = ReadValue(table, "texture_size", 100.f);
        if (textureSize == glm::vec3(0.0f)) textureSize = scale;
        node->Set("texture_size", textureSize);

        // Create all its child nodes and add them as children to this
        std::vector<LuaTable> nodes = table.Get<std::vector<LuaTable>>("nodes", true);
        for (auto n : nodes)
        {
            std::shared_ptr<MeshNode> new_node = CreateNode(id, renderPass, n);
            node->AddChild(new_node);
        }

        // Create the child cameras, if any
        std::vector<LuaTable> cameras = table.Get<std::vector<LuaTable>>("cameras", true);
        for (auto camera : cameras)
        {
            std::shared_ptr<Camera> newCamera = CreateCamera(id, camera);
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
        node->SetOrigin(ReadValue(table, "origin"));
        node->Set("position", ReadValue(table, "position", 100.f));
        node->Set("rotation", ReadValue(table, "rotation"));

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
