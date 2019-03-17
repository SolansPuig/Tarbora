#pragma once
#include "Logger.hpp"

namespace Tarbora {
    class Layer
    {
    public:
        Layer(bool start_active = true) : m_active(start_active) {}
        virtual ~Layer() {}

        virtual void OnActivate() {}
        virtual void OnDeactivate() {}

        virtual void OnDraw() {}

        void SetActive(bool active)
        {
            m_active = active;
            if (m_active) OnActivate();
            else OnDeactivate();
        }

        bool IsActive()
        {
            return m_active;
        }
    private:
        bool m_active;
    };
}
