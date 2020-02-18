#include "ModuleComponent.hpp"

namespace Tarbora {
    ModuleComponent::~ModuleComponent()
    {
        for(auto subscription : subscribed_messages_)
        {
            module_->getMessageManager()->desubscribe(subscription.first, subscription.second);
        }
    }

    void ModuleComponent::subscribe(const MessageSubject &s, MessageFn callback)
    {
        const auto element = subscribed_messages_.find(s);
        if (element == subscribed_messages_.end())
        {
            const SubscriptorId id = module_->getMessageManager()->subscribe(s, callback);
            subscribed_messages_[s] = id;
        }
    }

    void ModuleComponent::desubscribe(const MessageSubject &s)
    {
        const auto subscription = subscribed_messages_.find(s);
        if (subscription != subscribed_messages_.end())
        {
            module_->getMessageManager()->desubscribe(subscription->first, subscription->second);
            subscribed_messages_.erase(subscription);
        }
    }
}
