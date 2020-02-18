#pragma once
#include "../../../Framework/Module/Module.hpp"

namespace Tarbora {
    class ArduinoView : public Module
    {
    public:
        ArduinoView();
        ~ArduinoView();

        virtual void update(float delta_time) override;

    private:
        std::fstream arduino_;
    };
}
