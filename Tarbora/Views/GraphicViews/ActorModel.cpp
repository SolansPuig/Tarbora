#include "ActorModel.hpp"
#include "Scene.hpp"

namespace Tarbora {
    ActorModel::ActorModel(const ActorId &id, RenderPass render_pass, const std::string &model, const std::string &material)
        : MaterialNode(id, id, material)
    {
        model_ = model;
        render_pass_ = render_pass;
        ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
        for (auto n : resource->get("nodes"))
        {
            std::shared_ptr<MeshNode> new_node = createNode(id, render_pass, n.second.getAs<LuaTable>());
            addChild(new_node);
        }
        setGlobalScale(resource->get<float>("scale", 1.f, true));
    }

    ActorModel::~ActorModel()
    {
        nodes_.clear();
    }

    std::shared_ptr<MeshNode> ActorModel::createNode(const ActorId &id, RenderPass render_pass, LuaTable table)
    {
        // Read all the parameters for the node
        const std::string name = table.get<std::string>("name");
        const std::string shape = table.get<std::string>("shape");
        const bool animated = table.get<bool>("animated", false, true);

        // Create the node
        std::shared_ptr<MeshNode> node;
        if (animated)
            node = std::shared_ptr<AnimatedNode>(new AnimatedNode(id, name, render_pass, shape));
        else
            node = std::shared_ptr<MeshNode>(new MeshNode(id, name, render_pass, shape));

        node->setGlobalScale(table.get<float>("scale", 1.f, true));
        node->setOrigin(table.get<glm::vec3>("origin", true));
        node->setPosition(table.get<glm::vec3>("position", true)/100.f);
        node->setRotation(table.get<glm::vec3>("rotation", true));
        node->setUvMap(table.get<glm::vec3>("uv_map", true));
        node->setColorPrimary(table.get<glm::vec3>("color_primary", glm::vec3(255.f), true));
        node->setColorSecondary(table.get<glm::vec3>("color_secondary", glm::vec3(255.f), true));
        node->setColorDetail(table.get<glm::vec3>("color_detail1", glm::vec3(255.f), true));
        node->setColorDetail2(table.get<glm::vec3>("color_detail2", glm::vec3(255.f), true));
        const glm::vec3 scale = table.get<glm::vec3>("size", true)/100.f;
        node->setScale(scale);
        node->setMeshSize(scale);
        glm::vec3 texture_size = table.get<glm::vec3>("texture_size", true)/100.f;
        if (texture_size == glm::vec3(0.0f)) texture_size = scale;
        node->setTextureSize(texture_size);

        // Create all its child nodes and add them as children to this
        for (auto n : table.get("nodes", true))
        {
            LuaTable t = n.second.getAs<LuaTable>();
            std::string type = t.get<std::string>("type", "mesh", true);
            if (type == "mesh")
                node->addChild(createNode(id, render_pass, t));
            else if (type == "camera")
                node->addChild(createCamera(id, t));
        }

        nodes_[name] = node;

        return node;
    }

    std::shared_ptr<Camera> ActorModel::createCamera(const ActorId &id, LuaTable table)
    {
        // Read all the parameters for the node
        const std::string name = table.get<std::string>("name");

        // Create the node
        std::shared_ptr<Camera> node = std::shared_ptr<Camera>(new Camera(id, name));
        node->setOrigin(table.get<glm::vec3>("origin", true));
        node->setPosition(table.get<glm::vec3>("position", true)/100.f);
        node->setRotation(table.get<glm::vec3>("rotation", true));

        nodes_[name] = node;

        return node;
    }

    void ActorModel::update(Scene *scene, float delta_time)
    {
        if (animation_controller_)
        {
            animation_controller_->update(delta_time);
        }

        MaterialNode::update(scene, delta_time);
    }

    std::shared_ptr<SceneNode> ActorModel::getChild(const std::string &name)
    {
        auto itr = nodes_.find(name);
        if (itr != nodes_.end())
        {
            return itr->second;
        }
        return std::shared_ptr<SceneNode>();
    }

    void ActorModel::startAnimation(Animation animation, bool background)
    {
        if (!animation_controller_)
        {
            animation_controller_ = std::unique_ptr<AnimationController>(new AnimationController(this));
        }
        animation_controller_->startAnimation(animation, background);
    }

    void ActorModel::endAnimation(const std::string &name, StopMode mode, float fade_out_timer)
    {
        if (animation_controller_)
        {
            animation_controller_->endAnimation(name, mode, fade_out_timer);
        }
    }

    void ActorModel::setOutline(bool outline, const glm::tvec3<unsigned char> &color, float thickness)
    {
        for (auto node : nodes_)
        {
            if (node.second->getNodeType() == "MESH" || node.second->getNodeType() == "ANIMATED")
            {
                auto mesh = std::static_pointer_cast<MeshNode>(node.second);
                mesh->setOutline(outline, color, thickness);
            }
        }
    }
}
