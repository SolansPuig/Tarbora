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

#include "Tarbora/Views/GraphicViews/HumanView.hpp"
#include "Tarbora/Views/SceneManager/SceneManager.hpp"
#include "Tarbora/Logic/GameLogic.hpp"
#include "Tarbora/Messages/BasicMessages.hpp"
#include "Tarbora/Editor/Editor.hpp"
#include "Tarbora/Views/HardwareViews/ArduinoView.hpp"

#include "Demo/GravityRandomizerSystem.hpp"
#include "Demo/KnockbackSystem.hpp"
#include "Demo/ThrowSystem.hpp"
#include "Demo/Cockatiel.hpp"

using namespace Tarbora;

int main()
{
  ZoneScoped;
  LOG_LEVEL(DEBUG);

  ResourceManager::init("../Resources/");
    
  // auto message_bus = std::make_shared<MessageBus>("0.0.0.0:50051");
  // message_bus->runThread("Message Bus");

  auto logic = std::make_shared<World>();
  logic->registerSystem(std::make_shared<ItemDisplaySystem>(logic.get()));
  logic->registerSystem(std::make_shared<InventorySystem>(logic.get()));
  logic->registerSystem(std::make_shared<RenderSystem>(logic.get()));
  logic->registerSystem(std::make_shared<AnimationSystem>(logic.get()));
  logic->registerSystem(std::make_shared<EntitySystem>(logic.get()));
  logic->registerSystem(std::make_shared<PhysicsSystem>(logic.get()));
  logic->registerSystem(std::make_shared<ControllerSystem>(logic.get()));
  logic->registerSystem(std::make_shared<GrabSystem>(logic.get()));
  logic->registerSystem(std::make_shared<GravityRandomizerSystem>(logic.get()));
  logic->registerSystem(std::make_shared<KnockbackSystem>(logic.get()));
  logic->registerSystem(std::make_shared<ThrowSystem>(logic.get()));

  logic->runThread("Logic");

  auto arduino = std::make_shared<ArduinoView>();
  arduino->runThread("Arduino");

  auto human_view = std::make_shared<HumanView>();
  human_view->pushLayer(std::make_shared<Editor>(human_view.get(), false));

  // Draw the Sun
  auto sky = human_view->getGameLayer()->getScene()->getSkybox();
  sky->sun[0].color = {255, 247, 232};
  sky->sun[0].position = {-0.3f, 0.41f, -1.f};
  sky->sun[0].size = 4;
  sky->sun[0].sharpness = 18;
  sky->direction = glm::normalize(-sky->sun[0].position);

  auto scene_manager = std::make_shared<SceneManager>();
  scene_manager->load("dolmen.lua");

  auto cockatielIA = std::make_shared<CockatielIA>();
  cockatielIA->runThread("Cockatiel");

  human_view->run("Human View");

  ResourceManager::close();
  return 0;
}
