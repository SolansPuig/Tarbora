#pragma once

#include "AbstractModule.hpp"
#include "../MessageManager/MessageManager.hpp"
#include "../ResourceManager/Resource.hpp"
#include "../ResourceManager/Lua.hpp"

/**
 * @file
 * @brief Class @ref Tarbora::Module
*/

namespace Tarbora {
    //!The class that most modules inherit from.
    class Module : public AbstractModule
    {
        friend class ModuleComponent;
    public:
        //! Constructor
        /*!
            \param id This will be the id used by the MessageManager. This module will be identified
                by this id.
        */
        Module(const ClientId &id);
        ~Module();

        //! Start this module's loop.
        /*!
            The max_fps_ member should be set by child modules to limit their frames per second.
            \param name The name of the module, only used for the profiler.
        */
        virtual void run(const std::string &name="Unamed_Module");

        //! Read input, for example from keyboard and mouse.
        /*!
            When this function is overrided in a child module, it will be called at the begginnig
            of each iteration.
        */
        virtual void getInput() {}

        //! Perform the logic and the computations of the module.
        /*!
            This function must be overrided in every child module, it will be called after Module::getInput at each
            iteration.
            \param delta_time The elapsed time, in seconds, since the last iteration.
        */
        virtual void update(float delta_time) = 0;

        //! Render the data of the module.
        /*!
            When this function is overrided in a child module, it will be called at the end of each iteration.
        */
        virtual void draw() {}

    protected:
        std::shared_ptr<MessageManager> getMessageManager() const { return message_manager_; }

        uint max_fps_{60};
    private:
        std::chrono::high_resolution_clock::time_point time_;
        std::shared_ptr<MessageManager> message_manager_;
    };
}
