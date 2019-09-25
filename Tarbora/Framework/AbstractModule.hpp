#pragma once

#include "CommonHeaders.hpp"
#include "Global.hpp"

namespace Tarbora {

    //! Each of the parts that compose the Engine.
    class AbstractModule
    {
    public:
        AbstractModule()
            : m_UsingThread(false)
        {}

        ~AbstractModule();

        //! Start this module's loop.
        /*!
            It should stop when AbstractModule::m_Run is set to false.
        */
        virtual void Run(std::string name="Unamed Module") = 0;

        //! Start this module's loop in a new thread.
        void RunThread(std::string name="Unamed Module");

        //! Stop this module's loop.
        void Close();

    protected:
        static bool m_Run;
        bool m_UsingThread;
        
    private:
        std::thread m_Thread;
    };
}
