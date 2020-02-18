#pragma once

#include "AbstractModule.hpp"
#include "../MessageManager/MessageManager.hpp"
#include "../ResourceManager/Resource.hpp"
#include "../ResourceManager/Lua.hpp"

namespace Tarbora {
    class Module : public AbstractModule
    {
        friend class ModuleComponent;
    public:
        Module(const ClientId &id);
        ~Module();

        virtual void run(const std::string &name="Unamed Module");

        virtual void getInput() {}
        virtual void update(float delta_time) = 0;
        virtual void draw() {}

    protected:
        std::shared_ptr<MessageManager> getMessageManager() const { return message_manager_; }

        uint max_fps_;
    private:
        std::chrono::high_resolution_clock::time_point time_;
        std::shared_ptr<MessageManager> message_manager_;
    };
}
