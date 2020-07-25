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

#include "HumanView.hpp"

namespace Tarbora {
  HumanView::HumanView() :
    GraphicView(20, "Settings.lua")
  {
    LOG_DEBUG("Creating human game view...");

    max_fps_ = 60;

    getGraphicsEngine()->getInputManager()->captureMouse(true);

    metrics = std::make_shared<MetricsGui>(this, false);
    pushLayer(metrics);

    demo_gui = std::make_shared<DemoWindow>(this, false);
    pushLayer(demo_gui);

    LOG_DEBUG("Created");
  }

  HumanView::~HumanView()
  {
    LOG_DEBUG("Destroying human game view...");
  }

  void HumanView::getInput()
  {
    ZoneScoped;

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_ESCAPE))
    {
      getGraphicsEngine()->getInputManager()->toggleCaptureMouse();
    }

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F2))
    {
      getGraphicsEngine()->getWindow()->takeScreenshot("/home/roger/Imatges/test");
    }

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F3))
    {
      metrics->toggleActive();
    }

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F4))
    {
      getLayer("editor")->toggleActive();
    }

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F5))
    {
      ResourceManager::flush();
    }

    if (getGraphicsEngine()->getInputManager()->getKeyDown(KEY_F6))
    {
      demo_gui->toggleActive();
    }

    for (auto itr = layers_.rbegin(); itr != layers_.rend(); itr++)
    {
      if ((*itr)->isActive() && (*itr)->getInput())
        break;
    }

    getGraphicsEngine()->getInputManager()->update();
  }

  void HumanView::update(float delta_time)
  {
    ZoneScoped;

    for (auto itr = layers_.rbegin(); itr != layers_.rend(); itr++)
    {
      if ((*itr)->isActive())
        (*itr)->update(delta_time);
    }
  }

  void HumanView::draw()
  {
    {
      ZoneScopedN("Before Draw");
      getGraphicsEngine()->beforeDraw();
    }
    {
      ZoneScopedN("Draw");
      for (auto &itr : layers_)
      {
        if (itr->isActive())
          itr->draw();
      }
    }
    {
      ZoneScopedN("After Draw");
      getGraphicsEngine()->afterDraw();
    }
  }

  void HumanView::pushLayer(std::shared_ptr<Layer> layer)
  {
    layers_.push_back(layer);
    layer_map_.emplace(layer->getType(), layer);
  }

  void HumanView::removeLayer(std::shared_ptr<Layer> layer)
  {
    layers_.remove(layer);
    layer_map_.erase(layer->getType());
  }

  std::shared_ptr<Layer> HumanView::getLayer(const std::string &type)
  {
    auto itr = layer_map_.find(type);
    if (itr != layer_map_.end())
      return itr->second;
    else
      return std::shared_ptr<Layer>();
  }
}
