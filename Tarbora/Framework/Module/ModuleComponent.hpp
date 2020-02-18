#pragma once

#include "Module.hpp"

namespace Tarbora {
    class ModuleComponent
    {
    public:
        ModuleComponent(Module *m) : module_(m) {}
        ~ModuleComponent();

        virtual void update(float delta_time) { UNUSED(delta_time); };
        virtual void draw() {};

        inline void send(const ClientId &to, const MessageSubject &s, const MessageBody &b) const
        {
            module_->getMessageManager()->send(to, s, b);
        }

        inline void trigger(const MessageSubject &s, const MessageBody &b) const
        {
            module_->getMessageManager()->trigger(s, b);
        }

        inline void triggerLocal(const MessageSubject &s, const MessageBody &b) const
        {
            module_->getMessageManager()->triggerLocal(s, b);
        }

    protected:
        // Only module components should be able to subscribe and descubscribe
        void subscribe(const MessageSubject &s, MessageFn callback);
        void desubscribe(const MessageSubject &s);

        Module *module_;

    private:
        std::unordered_map<std::string, SubscriptorId> subscribed_messages_;
    };
}
