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
        GraphicsEngine(Module *module, std::string settingsFile);
        ~GraphicsEngine();

        void BeforeDraw();
        void AfterDraw();

        std::shared_ptr<Window> GetWindow() { return m_Window; }
        std::shared_ptr<RenderQueue> GetRenderQueue() { return m_RenderQueue; }
        std::shared_ptr<Input> GetInputManager() { return m_InputManager; }

        Module *GetModule() { return m_Module; }

    private:
        Module *m_Module;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<RenderQueue> m_RenderQueue;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<Input> m_InputManager;
        std::unique_ptr<Gui> m_Gui;
    };
}
