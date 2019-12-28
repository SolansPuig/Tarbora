#include "../inc/ActorModel.hpp"
#include "../inc/Scene.hpp"

namespace Tarbora {
    ActorModel::ActorModel(ActorId id, RenderPass renderPass, const std::string &model, const std::string &material)
        : MaterialNode(id, std::to_string(id), material)
    {
        ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
        m_PixelDensity = resource->Get<float>("pixel_density");
        float scale = resource->Get<float>("scale");
        std::shared_ptr<MeshNode> mesh = CreateNode(id, renderPass, resource->Get("root"), m_PixelDensity, resource->Get<float>("texture_size"));
        // mesh->SetGlobalScale(glm::vec3(scale, scale, scale));

        AddChild(mesh);
    }

    std::shared_ptr<MeshNode> ActorModel::CreateNode(ActorId id, RenderPass renderPass, LuaTable table, float pixelDensity, float textureSize)
    {
        // Read all the parameters for the node
        std::string name = table.Get<std::string>("name");
        std::string shape = table.Get<std::string>("shape");

        glm::vec3 origin = glm::vec3(
            table.Get("origin").Get<float>(1),
            table.Get("origin").Get<float>(2),
            table.Get("origin").Get<float>(3)
        );
        glm::vec3 position = glm::vec3(
            table.Get("position").Get<float>(1),
            table.Get("position").Get<float>(2),
            table.Get("position").Get<float>(3)
        );
        glm::vec3 rotation = glm::vec3(
            table.Get("rotation").Get<float>(1),
            table.Get("rotation").Get<float>(2),
            table.Get("rotation").Get<float>(3)
        );
        glm::vec3 size = glm::vec3(
            table.Get("size").Get<float>(1),
            table.Get("size").Get<float>(2),
            table.Get("size").Get<float>(3)
        );
        glm::vec3 shearA = glm::vec3(
            table.Get("shear", true).Get<float>(1, true),
            table.Get("shear", true).Get<float>(2, true),
            table.Get("shear", true).Get<float>(3, true)
        );
        glm::vec3 shearB = glm::vec3(
            table.Get("shear", true).Get<float>(4, true),
            table.Get("shear", true).Get<float>(5, true),
            table.Get("shear", true).Get<float>(6, true)
        );
        glm::vec3 texSize = glm::vec3(
            table.Get("texture_size", true).Get<float>(1, 0, true)*pixelDensity/textureSize,
            table.Get("texture_size", true).Get<float>(2, 0, true)*pixelDensity/textureSize,
            table.Get("texture_size", true).Get<float>(3, 0, true)*pixelDensity/textureSize
        );
        if (texSize == glm::vec3(0.0f)) texSize = size*pixelDensity/textureSize;
        glm::vec2 uv = glm::vec2(
            table.Get("uv").Get<float>(1)/textureSize,
            table.Get("uv").Get<float>(2)/textureSize
        );
        glm::vec3 colorPrimary = glm::vec3(
            table.Get("color_primary", true).Get<float>(1, 255, true)/255.0,
            table.Get("color_primary", true).Get<float>(2, 255, true)/255.0,
            table.Get("color_primary", true).Get<float>(3, 255, true)/255.0
        );
        glm::vec3 colorSecondary = glm::vec3(
            table.Get("color_secondary", true).Get<float>(1, 255, true)/255.0,
            table.Get("color_secondary", true).Get<float>(2, 255, true)/255.0,
            table.Get("color_secondary", true).Get<float>(3, 255, true)/255.0
        );
        glm::vec3 colorDetail = glm::vec3(
            table.Get("color_detail", true).Get<float>(1, 255, true)/255.0,
            table.Get("color_detail", true).Get<float>(2, 255, true)/255.0,
            table.Get("color_detail", true).Get<float>(3, 255, true)/255.0
        );
        glm::vec3 colorDetail2 = glm::vec3(
            table.Get("color_detail2", true).Get<float>(1, 255, true)/255.0,
            table.Get("color_detail2", true).Get<float>(2, 255, true)/255.0,
            table.Get("color_detail2", true).Get<float>(3, 255, true)/255.0
        );

        // Create the node
        std::shared_ptr<MeshNode> node = std::shared_ptr<MeshNode>(new MeshNode(id, name, renderPass, shape));
        node->SetUV(texSize, uv);
        node->SetOrigin(origin);
        node->Set("position", position);
        node->Set("scale", size);
        node->Set("rotation", rotation);
        // node->SetShear(shearA, shearB);
        node->Set("color_primary", colorPrimary);
        node->Set("color_secondary", colorSecondary);
        node->Set("color_detail1", colorDetail);
        node->Set("color_detail2", colorDetail2);

        // Create all its child nodes and add them as children to this
        std::vector<LuaTable> nodes = table.Get<std::vector<LuaTable>>("nodes", true);
        for (auto n : nodes)
        {
            std::shared_ptr<MeshNode> new_node = CreateNode(id, renderPass, n, pixelDensity, textureSize);
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

        glm::vec3 origin = glm::vec3(
            table.Get("origin").Get<float>(1),
            table.Get("origin").Get<float>(2),
            table.Get("origin").Get<float>(3)
        );
        glm::vec3 position = glm::vec3(
            table.Get("position").Get<float>(1),
            table.Get("position").Get<float>(2),
            table.Get("position").Get<float>(3)
        );
        glm::vec3 rotation = glm::vec3(
            table.Get("rotation").Get<float>(1),
            table.Get("rotation").Get<float>(2),
            table.Get("rotation").Get<float>(3)
        );

        // Create the node
        std::shared_ptr<Camera> node = std::shared_ptr<Camera>(new Camera(id, name));
        node->SetOrigin(origin);
        node->Set("position", position);
        node->Set("rotation", rotation);

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

    SceneNodePtr ActorModel::GetChild(const std::string &name)
    {
        auto itr = m_Nodes.find(name);
        if (itr != m_Nodes.end())
        {
            return itr->second;
        }
        return SceneNodePtr();
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
            m_AnimationController = std::make_optional<AnimationController>(this, animations_file);
        }

        m_AnimationController->SetAnimation(animation_name);
    }
}
