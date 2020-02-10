#include "../inc/ArduinoView.hpp"
#include "../../../Messages/BasicMessages.hpp"

namespace Tarbora {
    ArduinoView::ArduinoView()
        : Module(22)
    {
        LOG_INFO("Creating arduino view...");

        m_Arduino.open("/dev/ttyACM0");

        LOG_INFO("Successfully created arduino game view");
    }

    ArduinoView::~ArduinoView()
    {
        LOG_INFO("Destroying arduino game view...");
        m_Arduino.close();
        LOG_INFO("Successfully destroyed arduino game view");
    }

    void ArduinoView::Update(float elapsed_time)
    {
        int data = m_Arduino.get() / 5;
        if (data > 0)
        {
            GetMessageManager()->Trigger("apply_force", ApplyPhysics("test_cube", data, glm::vec3(0.f, 1.f, 0.f)));
        }
    }
}
