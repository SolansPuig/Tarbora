#include "Events.hpp"

namespace Tarbora {
    namespace Event {
        EventConnection<WindowCloseEvent> WindowClose;
        EventConnection<WindowResizeEvent> WindowResize;
        EventConnection<WindowFocusEvent> WindowFocus;
        EventConnection<WindowMoveEvent> WindowMove;
        EventConnection<WindowIconifyEvent> WindowIconify;
        EventConnection<KeyPressEvent> KeyPress;
        EventConnection<KeyReleaseEvent> KeyRelease;
        EventConnection<MouseButtonPressEvent> MouseButtonPress;
        EventConnection<MouseButtonReleaseEvent> MouseButtonRelease;
        EventConnection<MouseMoveEvent> MouseMove;
        EventConnection<MouseScrollEvent> MouseScroll;
    }
}
