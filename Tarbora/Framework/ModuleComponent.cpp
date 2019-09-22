#include "ModuleComponent.hpp"

namespace Tarbora {
    ModuleComponent::~ModuleComponent()
    {
        for(auto subscription : m_SubscribedMessages)
        {
            Desubscribe(subscription.first, subscription.second);
        }
    }

    void ModuleComponent::Subscribe(MessageSubject s, MessageFn callback)
    {
        auto element = m_SubscribedMessages.find(s);
        if (element == m_SubscribedMessages.end())
        {
            SubscriptorId id = m_Module->GetMessageManager()->Subscribe(s, callback);
            m_SubscribedMessages[s] = id;
        }
    }

    void ModuleComponent::Desubscribe(MessageSubject s)
    {
        auto subscription = m_SubscribedMessages.find(s);
        if (subscription != m_SubscribedMessages.end())
        {
            Desubscribe(subscription->first, subscription->second);
            m_SubscribedMessages.erase(subscription);
        }
    }
}
