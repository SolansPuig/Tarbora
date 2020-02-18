#include "../inc/ArduinoView.hpp"
#include "../../../Messages/BasicMessages.hpp"

namespace Tarbora {
    ArduinoView::ArduinoView()
        : Module(22)
    {
        LOG_INFO("Creating arduino view...");

        arduino_.open("/dev/ttyACM0");

        LOG_INFO("Successfully created arduino game view");
    }

    ArduinoView::~ArduinoView()
    {
        LOG_INFO("Destroying arduino game view...");
        arduino_.close();
        LOG_INFO("Successfully destroyed arduino game view");
    }

    void ArduinoView::update(float delta_time)
    {
        UNUSED(delta_time);

        int data = arduino_.get() / 5;
        if (data > 0)
        {
            getMessageManager()->trigger("apply_force", Message::ApplyPhysics("test_cube", glm::vec3(0.f, 1.f, 0.f) * (float)data));
        }
    }
}
