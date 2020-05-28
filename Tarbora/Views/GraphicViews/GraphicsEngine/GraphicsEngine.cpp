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

#include "GraphicsEngine.hpp"
#include "../../../Framework/ResourceManager/Lua.hpp"

namespace Tarbora {
  GraphicsEngine::GraphicsEngine(Module *module, const std::string &settings_file) :
    module_(module)
  {
    ResourceManager::registerLoader(std::make_shared<ShaderResourceLoader>());
    ResourceManager::registerLoader(std::make_shared<TextureResourceLoader>());
    ResourceManager::registerLoader(std::make_shared<MeshResourceLoader>());
    ResourceManager::registerLoader(std::make_shared<MaterialResourceLoader>());

    renderer_ = std::make_shared<Renderer>();

    ResourcePtr<LuaScript> settings(settings_file);
    LuaTable window = settings->get("window");
    std::string window_title = window.get<std::string>("title", "Tarbora Game Engine");
    float window_width = window.get("size").get<int>(1, 1280);
    float window_height = window.get("size").get<int>(2, 720);
    std::string postprocess = "shaders/postprocess.shader.lua";
    postprocess = window.get<std::string>("postprocessShader", postprocess);
    renderer_->setPostprocessShader(postprocess);

    window_ = std::make_unique<Window>(window_title, window_width, window_height, this);

    renderer_->init(window_width, window_height);
    render_queue_ = std::make_shared<RenderQueue>(renderer_);
    input_manager_ = std::make_shared<Input>(this);
    gui_ = std::make_shared<Gui>(this);
  }

  GraphicsEngine::~GraphicsEngine()
  {
    LOG_DEBUG("Destroying Graphics Engine");
  }

  void GraphicsEngine::beforeDraw()
  {
    gui_->beforeDraw();
  }

  void GraphicsEngine::afterDraw()
  {
    renderer_->postprocess();
    gui_->afterDraw();
    window_->update();
  }
}
