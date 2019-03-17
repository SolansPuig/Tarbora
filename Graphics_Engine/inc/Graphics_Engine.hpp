#pragma once
#include "Window.hpp"
#include "Layer.hpp"

namespace Tarbora {
    namespace Graphics_Engine {
        void Init();
        void Close();

        void BeforeDraw();
        void Draw();
        void AfterDraw();

        void AddLayer(Layer &layer);
        void AddOverlay(Layer &overlay);
        void AddGuiLayer(Layer &layer);
        void RemoveLayer(Layer &layer);
        void RemoveGuiOverlay(Layer &layer);

        inline void SetWindowTitle(const char* title);
        Window *GetWindow();
    }
}
