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
        virtual void Run() = 0;

        //! Start this module's loop in a new thread.
        void RunThread();

        //! Stop this module's loop.
        void Close();

    protected:
        static bool m_Run;

    private:
        std::thread m_Thread;
        bool m_UsingThread;
    };
}
