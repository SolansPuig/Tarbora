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
    : MaterialNode(id, id)
  {
    setMaterial(material);
    model_ = model;
    setRenderPass(render_pass);
    ResourcePtr<LuaScript> resource("models/" + model, "models/cube.lua");
    LuaTable actor = resource->get("actor");

    if (actor.valid())
      load(actor, &nodes_);
  }

  ActorModel::~ActorModel()
  {
    nodes_.clear();
  }

  void ActorModel::load(const LuaTable &table, NodeMap *map)
  {
    SceneNode::load(table, map);
    setGlobalScale(table.get<float>("scale", 1.f, true));
  }

  void ActorModel::write(LuaFile *file)
  {
    file->beginGlobalTable("actor");
    file->write("scale", getGlobalScale());
    writeChildren(file);
    file->closeGlobalTable();
  }

  void ActorModel::update(Scene *scene, float delta_time)
  {
    if (animation_controller_)
    {
      animation_controller_->update(delta_time);
    }

    MaterialNode::update(scene, delta_time);
  }

  SceneNodePtr ActorModel::getChild(const std::string &name)
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
