#pragma once
#include "GraphicView.hpp"

namespace Tarbora {
    class Layer
    {
    public:
        Layer(GraphicView *app, bool start_active = true) : app(app), m_active(start_active) {}
        virtual ~Layer() {}

        virtual void OnActivate() {}
        virtual void OnDeactivate() {}

        virtual void GetInput() {}
        virtual void Update(float elapsed_time) { (void)(elapsed_time); }
        virtual void Draw() {}

        virtual bool OnMessage(Message *m) { (void)(m); return false; }

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

    protected:
        GraphicView *app;
        
        bool m_active;
        bool m_event_blocking;
    };

    typedef std::list<std::shared_ptr<Layer>> LayerList;
}
