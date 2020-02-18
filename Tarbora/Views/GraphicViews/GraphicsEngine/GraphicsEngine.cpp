#include "GraphicsEngine.hpp"
#include "../../../Framework/ResourceManager/Lua.hpp"

namespace Tarbora {
    GraphicsEngine::GraphicsEngine(Module *module, const std::string &settings_file) :
        module_(module)
    {
        ResourceManager::registerLoader(std::shared_ptr<ResourceLoader>(new ShaderResourceLoader()));
        ResourceManager::registerLoader(std::shared_ptr<ResourceLoader>(new TextureResourceLoader()));
        ResourceManager::registerLoader(std::shared_ptr<ResourceLoader>(new MeshResourceLoader()));
        ResourceManager::registerLoader(std::shared_ptr<ResourceLoader>(new MaterialResourceLoader()));

        renderer_ = std::shared_ptr<Renderer>(new Renderer());

        ResourcePtr<LuaScript> settings(settings_file);
        LuaTable window = settings->get("window");
        std::string window_title = window.get<std::string>("title", "Tarbora Game Engine");
        float window_width = window.get("size").get<int>(1, 1280);
        float window_height = window.get("size").get<int>(2, 720);
        std::string postprocess_shader = "shaders/postprocess.shader.lua";
        renderer_->setPostprocessShader(window.get<std::string>("postprocessShader", postprocess_shader));

        window_ = std::unique_ptr<Window>(new Window(window_title, window_width, window_height, this));

        renderer_->init(window_width, window_height);
        render_queue_ = std::shared_ptr<RenderQueue>(new RenderQueue(renderer_));
        input_manager_ = std::shared_ptr<Input>(new Input(this));
        gui_ = std::shared_ptr<Gui>(new Gui(this));
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
