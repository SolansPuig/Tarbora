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

#ifndef __GRAPHICS_ENGINE_H_
#define __GRAPHICS_ENGINE_H_

#include "Gui.hpp"
#include "Input.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "RenderQueue.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Material.hpp"

namespace Tarbora {
  class Module;

  class GraphicsEngine {
  public:
    GraphicsEngine(Module *module, const std::string &settings_file);
    ~GraphicsEngine();

    void beforeDraw();
    void afterDraw();

    std::shared_ptr<Window> getWindow() { return window_; }
    std::shared_ptr<Renderer> getRenderer() { return renderer_; }
    std::shared_ptr<RenderQueue> getRenderQueue() { return render_queue_; }
    std::shared_ptr<Input> getInputManager() { return input_manager_; }
    Module *getModule() { return module_; }

  private:
    Module *module_;
    std::shared_ptr<Window> window_;
    std::shared_ptr<RenderQueue> render_queue_;
    std::shared_ptr<Renderer> renderer_;
    std::shared_ptr<Input> input_manager_;
    std::shared_ptr<Gui> gui_;
  };
}

#endif // __GRAPHICS_ENGINE_H_
