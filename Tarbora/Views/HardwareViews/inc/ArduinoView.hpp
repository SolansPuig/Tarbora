#pragma once
#include "../../../Framework/Module.hpp"

namespace Tarbora {
    class ArduinoView : public Module
    {
    public:
        ArduinoView(std::string server_address);
        ~ArduinoView();

        virtual void Update(float elapsed_time) override;

    private:
        std::fstream m_Arduino;
    };
}