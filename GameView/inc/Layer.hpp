#pragma once
#include <list>
#include <memory>
#include "Events.hpp"

namespace Tarbora {
    class Layer
    {
    public:
        Layer(bool start_active = true) : m_active(start_active) {}
        virtual ~Layer() {}

        virtual void OnActivate() {}
        virtual void OnDeactivate() {}

        virtual void Update(float elapsed_time) { (void)(elapsed_time); }
        virtual void Draw() {}

        virtual bool OnEvent(Event *e) { (void)(e); return false; }

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
        bool m_Event_blocking;
    };

    typedef std::list<std::shared_ptr<Layer>> LayerList;
}
