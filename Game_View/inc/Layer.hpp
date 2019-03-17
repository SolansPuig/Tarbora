#pragma once

#include <list>
#include <memory>

namespace Tarbora {
    class Layer
    {
    public:
        Layer(bool start_active = true) : m_active(start_active) {}
        virtual ~Layer() {}

        virtual void OnActivate() {}
        virtual void OnDeactivate() {}

        virtual void Draw() {}

        void SetActive(bool active)
        {
            m_active = active;
            if (m_active) OnActivate();
            else OnDeactivate();
        }

        bool IsActive() const
        {
            return m_active;
        }
    private:
        bool m_active;
    };

    typedef std::list<std::shared_ptr<Layer>> LayerList;
}
