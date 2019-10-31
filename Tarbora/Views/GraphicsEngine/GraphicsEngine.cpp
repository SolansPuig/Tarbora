#include "GraphicsEngine.hpp"
#include "../../Framework/ResourceManager/inc/Lua.hpp"

namespace Tarbora {
    GraphicsEngine::GraphicsEngine(Module *module, std::string settingsFile) :
        m_Module(module)
    {
        m_Renderer = std::shared_ptr<Renderer>(new Renderer());

        std::string windowTitle = "Tarbora Game Engine";
        int windowWidth = 1280, windowHeight = 720;
        std::string postprocessShader = "shaders/postprocess.shader.json";

        ResourcePtr<LuaScript> settings(settingsFile);

        LuaTable window = settings->Get("window");
        windowTitle = window.Get<std::string>("title", windowTitle);
        windowWidth = window.Get("size").Get<int>(1, windowWidth);
        windowHeight = window.Get("size").Get<int>(2, windowHeight);
        m_Renderer->SetPostprocessShader(window.Get<std::string>("postprocessShader", postprocessShader));

        m_Window = std::unique_ptr<Window>(new Window(windowTitle.c_str(), windowWidth, windowHeight, this));

        ResourceManager::RegisterLoader(LoaderPtr(new ShaderResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new TextureResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new MeshResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new MaterialResourceLoader()));

        m_Renderer->Init(windowWidth, windowHeight);
        m_RenderQueue = std::shared_ptr<RenderQueue>(new RenderQueue(m_Renderer));
        m_InputManager = std::shared_ptr<Input>(new Input(this));
        m_Gui = std::unique_ptr<Gui>(new Gui(this));
    }

    GraphicsEngine::~GraphicsEngine()
    {
        LOG_DEBUG("Destroying Graphics Engine");
    }

    void GraphicsEngine::BeforeDraw()
    {
        m_Gui->BeforeDraw();
    }

    void GraphicsEngine::AfterDraw()
    {
        m_Renderer->Postprocess();
        m_Gui->AfterDraw();
        m_Window->Update();
    }
}
