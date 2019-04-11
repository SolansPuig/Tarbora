#pragma once
#include <map>
#include <vector>
#include <functional>
#include "glm/glm.hpp"
#include "Logger.hpp"

namespace Tarbora {
    enum EventType
    {
        WindowClose, WindowResize, WindowFocus, WindowMove, WindowIconify,
        KeyPress, KeyRelease,
        MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll,
        ActorMove,
        CreateActor, CreateActorModel
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

    struct ActorEvent : public Event
    {
        ActorEvent(unsigned int id) : actorId(id) {}
        unsigned long int actorId;
    };

    struct ActorMoveEvent : public ActorEvent
    {
        ActorMoveEvent(unsigned int id, glm::vec3 p, glm::mat3 r) :
            ActorEvent(id), position(p), rotation(r) {}
        EventType GetType() override { return EventType::ActorMove; }
        glm::vec3 position;
        glm::mat3 rotation;
    };

    struct CreateActorEvent : public Event
    {
        CreateActorEvent(std::string e) :
            entity(e) {}
        EventType GetType() override { return EventType::CreateActor; }
        std::string entity;
    };

    struct CreateActorModelEvent : public Event
    {
        CreateActorModelEvent(unsigned int id, int pass, std::string m, std::string s, std::string t) :
            actorId(id), renderPass(pass), model(m), shader(s), texture(t) {}
        EventType GetType() override { return EventType::CreateActorModel; }
        unsigned int actorId;
        int renderPass;
        std::string model, shader, texture;
    };

    typedef std::function<void(Event*)> EventFn;
    typedef std::function<void(Event*)> CommandFn;
    typedef unsigned int EventId;
    typedef unsigned int CommandId;

    namespace EventManager
    {
        typedef std::vector<EventFn> EventListenerList;
        typedef std::map<EventType, EventListenerList> EventListenerMap;

        EventId Subscribe(EventType type, EventFn func);
        void Unsubscribe(EventType type, EventId id);
        void Trigger(EventType type, Event *event);
    };
}
