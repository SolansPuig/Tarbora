#pragma once
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
