#include "Graphics_Engine.hpp"
#include <memory>
#include "Logger.hpp"
#include "Gui.hpp"
#include "Layer_Stack.hpp"

namespace Tarbora {
    namespace Graphics_Engine {
        std::unique_ptr<Window> Main_Window;
        std::unique_ptr<Gui> m_Gui;
        Layer_Stack m_layer_stack;
        Layer_Stack m_gui_layer_stack;

        void Init()
        {
            glfwInit();
            Main_Window = std::unique_ptr<Window>(new Window("Tarbora Game Engine", 1280, 720));

            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) {
                LOG_ERR("Graphics_Engine: Failed to initialize GLEW");
                glfwTerminate();
            }

            m_Gui = std::unique_ptr<Gui>(new Gui());
        }

        void Close()
        {
            glfwTerminate();
        }

        void BeforeDraw()
        {
            Main_Window->Clear(0.0f, 0.0f, 0.0f);
            m_Gui->BeforeDraw();
        }

        void Draw()
        {
            for (Layer *layer : m_layer_stack)
            {
                if (layer->IsActive())
                    layer->OnDraw();
            }

            for (Layer *layer : m_gui_layer_stack)
            {
                if (layer->IsActive())
                    layer->OnDraw();
            }
        }

        void AfterDraw()
        {
            m_Gui->AfterDraw();
            Main_Window->Update();
        }

        void SetWindowTitle(const char* title)
        {
            Main_Window->SetTitle(title);
        }

        Window *GetWindow()
        {
            return static_cast<Window*>(Main_Window.get());
        }

        void AddLayer(Layer &layer)
        {
            m_layer_stack.AddLayer(layer);
        }

        void AddOverlay(Layer &overlay)
        {
            m_layer_stack.AddOverlay(overlay);
        }

        void AddGuiLayer(Layer &layer)
        {
            m_gui_layer_stack.AddLayer(layer);
        }

        void RemoveLayer(Layer &layer)
        {
            m_layer_stack.RemoveLayer(layer);
        }

        void RemoveGuiOverlay(Layer &layer)
        {
            m_gui_layer_stack.RemoveLayer(layer);
        }

    }
}
