#pragma once

#include "AbstractModule.hpp"
#include "MessageServer.hpp"

namespace Tarbora {

    //! The module that makes possible the communication between modules in different processes or even machines.
    class MessageBus : public AbstractModule
    {
    public:
        //! Create the message bus.
        /*!
            \param serverAddress The IP address where the message bus runs.
        */
        MessageBus(const std::string serverAddress) { m_Server = std::unique_ptr<MessageServer>(new MessageServer(serverAddress)); }
        ~MessageBus() {}

        //! Start the message bus loop.
        /*!
            It should stop when AbstractModule::m_Run is set to false.
        */
        virtual void Run(std::string name="Unamed Module") override { m_Server->Run(); }

    private:
        std::unique_ptr<MessageServer> m_Server;
    };
}
