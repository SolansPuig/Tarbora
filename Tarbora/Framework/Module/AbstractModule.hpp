#pragma once

#include "../CommonHeaders.hpp"
#include "../Global.hpp"

namespace Tarbora {

    //! Each of the parts that compose the Engine.
    class AbstractModule
    {
    public:
        AbstractModule() : using_thread_(false){}

        ~AbstractModule();

        //! Start this module's loop.
        /*!
            All modules should stop when AbstractModule::run_ is set to false.
            \param name The name of the module, only used for the profiler.
        */
        virtual void run(const std::string &name="Unamed Module") = 0;

        //! Start this module's loop in a new thread.
        /*!
            \param name The name of the module, only used for the profiler.
        */
        void runThread(const std::string &name="Unamed Module");

        //! Stop this module's loop.
        void close();

    protected:
        static bool run_;
        bool using_thread_;

    private:
        std::thread thread_;
    };
}
