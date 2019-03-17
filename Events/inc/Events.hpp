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

    template <typename T>
    class EventConnection
    {
        using EventFn = std::function<void(T&)>;
        std::vector<EventFn> m_Observers;
    public:
        EventConnection()
        {}

        unsigned int Subscribe(const EventFn func)
        {
            m_Observers.push_back(func);
            return m_Observers.size()-1;
        }

        void Unsubscribe(unsigned int i)
        {
            m_Observers.erase(m_Observers.begin() + i);
        }

        void Trigger(T& e)
        {
            for (auto itr=m_Observers.begin(); itr != m_Observers.end(); itr++)
            {
                (*itr)(e);
            }
        }
    };

    struct WindowCloseEvent
    {
        WindowCloseEvent() {}
    };

    struct WindowResizeEvent
    {
        WindowResizeEvent(int w, int h) : width(w), height(h) {}
        int width, height;
    };

    struct WindowFocusEvent
    {
        WindowFocusEvent(int f) : focused(f) {}
        char focused;
    };

    struct WindowMoveEvent
    {
        WindowMoveEvent(int nx, int ny) : x(nx), y(ny) {}
        int x, y;
    };

    struct WindowIconifyEvent
    {
        WindowIconifyEvent(int ic) : iconified(ic) {}
        int iconified;
    };

    struct KeyPressEvent
    {
        KeyPressEvent(int k, int m, int r) : key(k), mods(m), repeat(r) {}
        int key, mods, repeat;
    };

    struct KeyReleaseEvent
    {
        KeyReleaseEvent(int k, int m) : key(k), mods(m) {}
        int key, mods;
    };

    struct MouseButtonPressEvent
    {
        MouseButtonPressEvent(int b, int m) : button(b), mods(m) {}
        int button, mods;
    };

    struct MouseButtonReleaseEvent
    {
        MouseButtonReleaseEvent(int b, int m) : button(b), mods(m) {}
        int button, mods;
    };

    struct MouseMoveEvent
    {
        MouseMoveEvent(int nx, int ny) : x(nx), y(ny) {}
        int x, y;
    };

    struct MouseScrollEvent
    {
        MouseScrollEvent(int nx, int ny) : xoffset(nx), yoffset(ny) {}
        int xoffset, yoffset;
    };

    namespace Event {
        extern EventConnection<WindowCloseEvent> WindowClose;
        extern EventConnection<WindowResizeEvent> WindowResize;
        extern EventConnection<WindowFocusEvent> WindowFocus;
        extern EventConnection<WindowMoveEvent> WindowMove;
        extern EventConnection<WindowIconifyEvent> WindowIconify;
        extern EventConnection<KeyPressEvent> KeyPress;
        extern EventConnection<KeyReleaseEvent> KeyRelease;
        extern EventConnection<MouseButtonPressEvent> MouseButtonPress;
        extern EventConnection<MouseButtonReleaseEvent> MouseButtonRelease;
        extern EventConnection<MouseMoveEvent> MouseMove;
        extern EventConnection<MouseScrollEvent> MouseScroll;
    }
}
