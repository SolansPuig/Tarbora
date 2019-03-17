#pragma once
#include "Layer.hpp"
#include <vector>

namespace Tarbora {
    class Layer_Stack
    {
    public:
        Layer_Stack();
        ~Layer_Stack();

        void AddLayer(Layer &layer);
        void AddOverlay(Layer &overlay);
        void RemoveLayer(Layer &layer);
        void RemoveOverlay(Layer &overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }
    private:
        std::vector<Layer*> m_layers;
        std::vector<Layer*>::iterator m_layerInsert;
    };
}
