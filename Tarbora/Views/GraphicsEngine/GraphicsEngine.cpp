#include "GraphicsEngine.hpp"
#include "../../Framework/ResourceManager/inc/Json.hpp"

namespace Tarbora {
    GraphicsEngine::GraphicsEngine(Module *module, std::string settingsFile) :
        m_Module(module)
    {
        m_Renderer = std::shared_ptr<Renderer>(new Renderer());

        std::string windowTitle = "Tarbora Game Engine";
        int windowWidth = 1280, windowHeight = 720;

        JsonPtr settings = GET_RESOURCE(Json, settingsFile);
        if (settings)
        {
            raw_json window;
            settings->Get("window", &window, {true});
            if (!window.empty())
            {
                settings->PushErrName("window");

                settings->Get(window, "title", &windowTitle, {true});

                settings->GetArray(window, "size", 0, &windowWidth, {true});
                settings->GetArray(window, "size", 1, &windowHeight, {true});

                settings->PopErrName();
            }
        };

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
