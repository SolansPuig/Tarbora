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

#include "Scene.hpp"
#include "HumanView.hpp"
#include "Skybox.hpp"
#include "ActorModel.hpp"
#include <utility>

namespace Tarbora {
  Scene::Scene(HumanView *view) :
    view_(view)
  {
    root_ = std::shared_ptr<RootNode>(new RootNode());
  }

  void Scene::update(float delta_time)
  {
    if (root_)
      root_->update(this, delta_time);
  }

  void Scene::draw()
  {
    projection_ = glm::perspective(
      glm::radians(45.0f), getGraphicsEngine()->getWindow()->getRatio(), 0.1f, 100.0f
    );
    getRenderQueue()->setProjectionMatrix(projection_);
    view_matrix_ = camera_->getView();
    getRenderQueue()->setViewMatrix(view_matrix_);

    if (root_)
    {
      root_->drawChildren(this);
      getRenderQueue()->draw();
    }
  }

  std::shared_ptr<Skybox> Scene::createSkybox(const std::string &material)
  {
    skybox_ = std::make_shared<Skybox>(material);
    addActor(skybox_);
    return skybox_;
  }

  std::shared_ptr<ActorModel> Scene::createActorModel(
    const ActorId &id, RenderPass render_pass,
    const std::string &model, const std::string &material
  )
  {
    if (getActor(id))
      removeActor(id);

    auto actor = std::make_shared<ActorModel>(id, render_pass, model, material);
    addActor(actor);
    return actor;
  }

  std::shared_ptr<Camera> Scene::createCamera(const ActorId &id)
  {
    std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera(id, "body"));
    if (camera)
      addActor(camera);
    return camera;
  }

  void Scene::setCamera(const ActorId &id, const std::string &name)
  {
    std::shared_ptr<SceneNode> child = getActor(id);
    if (child)
    {
      std::shared_ptr<SceneNode> camera = child->getChild(name);
      if (camera)
      {
        camera_ = std::static_pointer_cast<Camera>(camera);
      }
    }
  }

  void Scene::addActor(std::shared_ptr<SceneNode> actor)
  {
    const ActorId &id = actor->owner;
    if (id != "")
      actor_map_[id] = actor;
    root_->addChild(actor);
  }

  std::shared_ptr<SceneNode> Scene::getActor(const ActorId &id)
  {
    auto itr = actor_map_.find(id);
    if (itr == actor_map_.end())
    {
      return std::shared_ptr<SceneNode>();
    }
    return itr->second;
  }

  bool Scene::removeActor(const ActorId &id)
  {
    if (id != "")
    {
      actor_map_.erase(id);
      return root_->removeChild(id);
    }
    return false;
  }
}
