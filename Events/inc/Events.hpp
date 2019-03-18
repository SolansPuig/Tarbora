#pragma once
#include <map>
#include <vector>
#include <functional>
#include "Logger.hpp"

#define BIND(x) std::bind(&x, this, std::placeholders::_1)

namespace Tarbora {
    enum EventType
    {
        WindowClose, WindowResize, WindowFocus, WindowMove, WindowIconify,
        KeyPress, KeyRelease,
        MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
    };

    struct Event
    {
        Event() {}
        virtual EventType GetType() = 0;
    };

    struct WindowCloseEvent : public Event
    {
        WindowCloseEvent() {}
        EventType GetType() override { return EventType::WindowClose; }
    };

    struct WindowResizeEvent : public Event
    {
        WindowResizeEvent(int w, int h) : width(w), height(h) {}
        EventType GetType() override { return EventType::WindowResize; }
        int width, height;
    };

    struct WindowFocusEvent : public Event
    {
        WindowFocusEvent(int f) : focused(f) {}
        EventType GetType() override { return EventType::WindowFocus; }
        char focused;
    };

    struct WindowMoveEvent : public Event
    {
        WindowMoveEvent(int nx, int ny) : x(nx), y(ny) {}
        EventType GetType() override { return EventType::WindowMove; }
        int x, y;
    };

    struct WindowIconifyEvent : public Event
    {
        WindowIconifyEvent(int ic) : iconified(ic) {}
        EventType GetType() override { return EventType::WindowIconify; }
        int iconified;
    };

    struct KeyPressEvent : public Event
    {
        KeyPressEvent(int k, int m, int r) : key(k), mods(m), repeat(r) {}
        EventType GetType() override { return EventType::KeyPress; }
        int key, mods, repeat;
    };

    struct KeyReleaseEvent : public Event
    {
        KeyReleaseEvent(int k, int m) : key(k), mods(m) {}
        EventType GetType() override { return EventType::KeyRelease; }
        int key, mods;
    };

    struct MouseButtonPressEvent : public Event
    {
        MouseButtonPressEvent(int b, int m) : button(b), mods(m) {}
        EventType GetType() override { return EventType::MouseButtonPress; }
        int button, mods;
    };

    struct MouseButtonReleaseEvent : public Event
    {
        MouseButtonReleaseEvent(int b, int m) : button(b), mods(m) {}
        EventType GetType() override { return EventType::MouseButtonRelease; }
        int button, mods;
    };

    struct MouseMoveEvent : public Event
    {
        MouseMoveEvent(int nx, int ny) : x(nx), y(ny) {}
        EventType GetType() override { return EventType::MouseMove; }
        int x, y;
    };

    struct MouseScrollEvent : public Event
    {
        MouseScrollEvent(int nx, int ny) : xoffset(nx), yoffset(ny) {}
        EventType GetType() override { return EventType::MouseScroll; }
        int xoffset, yoffset;
    };

    typedef std::function<void(Event*)> EventFn;
    typedef unsigned int EventId;

    namespace EventManager
    {
        typedef std::vector<EventFn> EventListenerList;
        typedef std::map<EventType, EventListenerList> EventListenerMap;

        EventId Subscribe(EventType type, EventFn func);
        void Unsubscribe(EventType type, EventId id);
        void Trigger(EventType type, Event* event);
    };
}
