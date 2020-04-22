#pragma once

#include "MessageClient.hpp"

/** @dir MessageManager
 * @brief The message distribution infrastructure
*/
/**
 * @file
 * @brief Class @ref Tarbora::MessageManager
*/

/*!
    \page message_tutorial The Messaging System
*/
#define MSGBIND(T) (std::bind(T, this, std::placeholders::_1, std::placeholders::_2))

namespace Tarbora {
    //! The base for all the messages.
    /*!
        See @ref message_tutorial
    */
    class MessageBody
    {
    public:
        //! Constructor from a parsed string
        MessageBody(const std::string &body)
            : body_(body) {}

        MessageBody() {}

        //! Get the parsed string
        const std::string& getContent() const
        {
            return body_;
        }

    protected:
        std::string body_;
    };

    typedef std::function<void(const MessageSubject&, const MessageBody&)> MessageFn;

    //! The abstraction layer that manages all the messages from and for a module.
    /*!
        See @ref message_tutorial
    */
    class MessageManager
    {
    public:
        //! Constructor
        /*!
            \param id The id of the module that owns this.
        */
        MessageManager(const ClientId &id);
        ~MessageManager();

        inline void setDebugName(std::string name="Unamed Message Client")
        {
            message_client_->setDebugName(name);
        }

        void readMessages();

        SubscriptorId subscribe(const MessageSubject &s, MessageFn func, bool local=false);
        void desubscribe(const MessageSubject &s, const SubscriptorId &id);

        void trigger(const MessageSubject &s, const MessageBody &b);
        void send(const ClientId &to, const MessageSubject &s, const MessageBody &b);
        void triggerLocal(const MessageSubject &s, const MessageBody &b);

    private:
        void readMessage(const Message::Message &m);

        std::unique_ptr<MessageClient> message_client_;

        std::unordered_map<std::string, std::vector<MessageFn>> listeners_;
    };
}
