#pragma once
#include "../../../Framework/ClientApplication.hpp"

namespace Tarbora {
    class ArduinoView : public ClientApplication
    {
    public:
        ArduinoView(std::string server_address);
        ~ArduinoView();

        virtual void Update(float elapsed_time) override;

    private:
        std::fstream m_Arduino;
    };
}
