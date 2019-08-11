#include "../inc/ActorModel.hpp"
#include "../inc/Scene.hpp"

namespace Tarbora {
    ActorModel::ActorModel(ActorId id, std::string model, std::string shader, std::string texture)
        : MaterialNode(id, std::to_string(id), shader, texture)
    {
        JsonPtr resource = GET_RESOURCE(Json, "models/" + model);
        if (resource != NULL)
        {
            resource->PushErrName("root");
            m_PixelDensity = resource->GetFloat("pixel_density");
            std::shared_ptr<MeshNode> mesh = CreateNode(id, resource, resource->GetJson("root"), m_PixelDensity, resource->GetFloat("texture_size"));
            resource->PopErrName();

            AddChild(mesh);
        }
    }

    std::shared_ptr<MeshNode> ActorModel::CreateNode(ActorId id, JsonPtr resource, raw_json j, float pixelDensity, float textureSize)
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
        std::shared_ptr<MeshNode> node = std::shared_ptr<MeshNode>(new MeshNode(id, name, shape));
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
            std::shared_ptr<MeshNode> new_node = CreateNode(id, resource, resource->GetJson(nodes, i), pixelDensity, textureSize);
            resource->PopErrName();
            node->AddChild(new_node);
        }
        resource->PopErrName();

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
