#pragma once
#include "../../Framework/Module/ModuleComponent.hpp"
#include "GraphicView.hpp"

namespace Tarbora {
    class Layer : public ModuleComponent
    {
    public:
        Layer(GraphicView *view, bool start_active = true) :  ModuleComponent(view), active_(start_active) {}
        virtual ~Layer() {}

        virtual void onActivate() {}
        virtual void onDeactivate() {}

        virtual void getInput() {}
        virtual void update(float delta_time) { (void)(delta_time); }
        virtual void draw() {}

        virtual bool onMessage(const MessageBody &m) { (void)(m); return false; }

        void setActive(bool active)
        {
            active_ = active;
            if (active_) onActivate();
            else onDeactivate();
        }

        bool isActive() const
        {
            return active_;
        }

        std::shared_ptr<Input> getInputManager()
        {
            return static_cast<GraphicView*>(module_)->getGraphicsEngine()->getInputManager();
        }

    protected:
        bool active_;
        bool event_blocking_;
    };
}
