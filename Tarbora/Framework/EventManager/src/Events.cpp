#include "../inc/EventManager.hpp"

namespace Tarbora {
    namespace EventManager
    {
        EventListenerMap m_Listeners;

        EventId Subscribe(EventType type, EventFn func)
        {
            m_Listeners[type].push_back(func);
            return m_Listeners[type].size() -1;
        }

        void Unsubscribe(EventType type, EventId id)
        {
            m_Listeners[type].erase(m_Listeners[type].begin() + id);
        }

        void Trigger(EventType type, Event *event)
        {
            for (auto itr=m_Listeners[type].begin(); itr != m_Listeners[type].end(); itr++)
            {
                (*itr)(event);
            }
        }
    };
}
