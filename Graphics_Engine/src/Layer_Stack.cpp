#include "Layer_Stack.hpp"
#include "Logger.hpp"
#include <algorithm>

namespace Tarbora {
    Layer_Stack::Layer_Stack()
    {
        m_layerInsert = m_layers.begin();
    }

    Layer_Stack::~Layer_Stack()
    {
    }

    void Layer_Stack::AddLayer (Layer& layer)
    {
        m_layerInsert = m_layers.emplace(m_layerInsert, &layer);
    }

    void Layer_Stack::AddOverlay (Layer& overlay)
    {
        m_layers.emplace_back(&overlay);
    }

    void Layer_Stack::RemoveLayer (Layer &layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.end(), &layer);
        if (it != m_layers.end())
        {
            m_layers.erase(it);
        }
    }
}
