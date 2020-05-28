/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#include "ActorModel.hpp"
#include "Scene.hpp"

namespace Tarbora {
  ActorModel::ActorModel(
    const ActorId &id, RenderPass render_pass,
    const std::string &model, const std::string &material
  )
    : MaterialNode(id, id, material)
  {
    model_ = model;
    render_pass_ = render_pass;
    ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
    for (auto n : resource->get("nodes"))
    {
      auto new_node = createNode(id, render_pass, n.second.getAs<LuaTable>());
      addChild(new_node);
    }
    setGlobalScale(resource->get<float>("scale", 1.f, true));
  }

  ActorModel::~ActorModel()
  {
    nodes_.clear();
  }

  std::shared_ptr<MeshNode> ActorModel::createNode(
    const ActorId &id, RenderPass render_pass, LuaTable table
  )
  {
    // Read all the parameters for the node
    const std::string name = table.get<std::string>("name");
    const std::string shape = table.get<std::string>("shape");
    const bool animated = table.get<bool>("animated", false, true);

    // Create the node
    std::shared_ptr<MeshNode> node;
    if (animated)
      node = std::make_shared<AnimatedNode>(id, name, render_pass, shape);
    else
      node = std::make_shared<MeshNode>(id, name, render_pass, shape);

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
    glm::vec3 mesh_size = table.get<glm::vec3>("mesh_size", true)/100.f;
    if (mesh_size == glm::vec3(0.0f)) mesh_size = scale;
    node->setMeshSize(mesh_size);
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
    std::shared_ptr<Camera> node = std::make_shared<Camera>(id, name);
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
      animation_controller_ = std::make_unique<AnimationController>(this);
    }
    animation_controller_->startAnimation(animation, background);
  }

  void ActorModel::endAnimation(
    const std::string &name, StopMode mode, float fade_out_timer
  )
  {
    if (animation_controller_)
    {
      animation_controller_->endAnimation(name, mode, fade_out_timer);
    }
  }
}
