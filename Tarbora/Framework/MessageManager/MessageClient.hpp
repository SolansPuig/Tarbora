/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#ifndef __MESSAGECLIENT_H_
#define __MESSAGECLIENT_H_

#include "../CommonHeaders.hpp"
#include "../Global.hpp"

namespace Message { class Message; }

namespace Tarbora {
  class MessageClient {
  public:
    MessageClient(const ClientId &id);

    void connect();
    void disconnect();

    bool getMessage(Message::Message *message);
    void send(const Message::Message &message);
    void receive(const Message::Message &message);

    void subscribe(const MessageSubject &s);
    void desubscribe(const MessageSubject &s);

    inline void setDebugName(std::string /*name="Unamed Message Client"*/)
    {
      // TODO
    }

  private:
    std::queue<Message::Message> messages_;

    ClientId id_;
  };
}

#endif // __MESSAGECLIENT_H_
