#include "../inc/ActorModel.hpp"
#include "../inc/Scene.hpp"

namespace Tarbora {
    ActorModel::ActorModel(ActorId id, RenderPass renderPass, std::string model, std::string material)
        : MaterialNode(id, std::to_string(id), material)
    {
        JsonPtr resource = GET_RESOURCE(Json, "models/" + model);
        if (resource != NULL)
        {
            resource->PushErrName("root");
            m_PixelDensity = resource->GetFloat("pixel_density");
            float scale = resource->GetFloat("scale");
            std::shared_ptr<MeshNode> mesh = CreateNode(id, renderPass, resource, resource->GetJson("root"), m_PixelDensity, resource->GetFloat("texture_size"));
            mesh->SetGlobalScale(glm::vec3(scale, scale, scale));
            resource->PopErrName();

            AddChild(mesh);
        }
    }

    std::shared_ptr<MeshNode> ActorModel::CreateNode(ActorId id, RenderPass renderPass, JsonPtr resource, raw_json j, float pixelDensity, float textureSize)
    {
        // Read all the parameters for the node
        std::string name = resource->GetString(j, "name");
        std::string shape = resource->GetString(j, "shape");

        glm::vec3 origin = glm::vec3(
            resource->GetFloatArray(j, "origin", 0),
            resource->GetFloatArray(j, "origin", 1),
            resource->GetFloatArray(j, "origin", 2)
        );
        glm::vec3 position = glm::vec3(
            resource->GetFloatArray(j, "position", 0)/pixelDensity,
            resource->GetFloatArray(j, "position", 1)/pixelDensity,
            resource->GetFloatArray(j, "position", 2)/pixelDensity
        );
        glm::vec3 rotation = glm::vec3(
            resource->GetFloatArray(j, "rotation", 0),
            resource->GetFloatArray(j, "rotation", 1),
            resource->GetFloatArray(j, "rotation", 2)
        );
        glm::vec3 size = glm::vec3(
            resource->GetFloatArray(j, "size", 0)/pixelDensity,
            resource->GetFloatArray(j, "size", 1)/pixelDensity,
            resource->GetFloatArray(j, "size", 2)/pixelDensity
        );
        glm::vec3 texSize = glm::vec3(
            resource->GetFloatArray(j, "size", 0)/textureSize,
            resource->GetFloatArray(j, "size", 1)/textureSize,
            resource->GetFloatArray(j, "size", 2)/textureSize
        );
        glm::vec2 uv = glm::vec2(
            resource->GetFloatArray(j, "uv", 0)/textureSize,
            resource->GetFloatArray(j, "uv", 1)/textureSize
        );

        // Create the node
        std::shared_ptr<MeshNode> node = std::shared_ptr<MeshNode>(new MeshNode(id, name, renderPass, shape));
        node->SetUV(texSize, uv);
        node->SetOrigin(origin);
        node->SetPosition(position);
        node->SetScale(size);
        node->SetRotation(rotation);

        // Create all its child nodes and add them as children to this
        raw_json nodes;
        resource->Get(j, "nodes", &nodes, {true, true});
        resource->PushErrName("nodes");
        for (unsigned int i = 0; i < nodes.size(); i++) {
            resource->PushErrName(std::to_string(i).c_str());
            std::shared_ptr<MeshNode> new_node = CreateNode(id, renderPass, resource, resource->GetJson(nodes, i), pixelDensity, textureSize);
            resource->PopErrName();
            node->AddChild(new_node);
        }
        resource->PopErrName();

        // Create the child cameras, if any
        raw_json cameras;
        resource->Get(j, "cameras", &cameras, {true, true});
        resource->PushErrName("cameras");
        for (unsigned int i = 0; i < cameras.size(); i++) {
            resource->PushErrName(std::to_string(i).c_str());
            std::shared_ptr<Camera> newCamera = CreateCamera(id, resource, resource->GetJson(cameras, i));
            resource->PopErrName();
            node->AddChild(newCamera);
        }
        resource->PopErrName();

        m_Nodes[name] = node;

        return node;
    }

    std::shared_ptr<Camera> ActorModel::CreateCamera(ActorId id, JsonPtr resource, raw_json j)
    {
        // Read all the parameters for the node
        std::string name = resource->GetString(j, "name");

        glm::vec3 origin = glm::vec3(
            resource->GetFloatArray(j, "origin", 0),
            resource->GetFloatArray(j, "origin", 1),
            resource->GetFloatArray(j, "origin", 2)
        );
        glm::vec3 position = glm::vec3(
            resource->GetFloatArray(j, "position", 0),
            resource->GetFloatArray(j, "position", 1),
            resource->GetFloatArray(j, "position", 2)
        );
        glm::vec3 rotation = glm::vec3(
            resource->GetFloatArray(j, "rotation", 0),
            resource->GetFloatArray(j, "rotation", 1),
            resource->GetFloatArray(j, "rotation", 2)
        );

        // Create the node
        std::shared_ptr<Camera> node = std::shared_ptr<Camera>(new Camera(id, name));
        node->SetOrigin(origin);
        node->SetPosition(position);
        node->SetRotation(rotation);

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

    SceneNodePtr ActorModel::GetChild(std::string name)
    {
        auto itr = m_Nodes.find(name);
        if (itr != m_Nodes.end())
        {
            return itr->second;
        }
        return SceneNodePtr();
    }

    void ActorModel::Animate(std::string animation_name, std::string animations_file)
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
