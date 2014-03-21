/*Copyright (c) <2014> Kevin Miller - KevM1227@gmail.com

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#ifndef _MESSAGEHUB_H
#define _MESSAGEHUB_H

#include <iostream>
#include <memory>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#include "OCS/Messaging/Message.hpp"
#include "OCS/Messaging/Transceiver.hpp"
#include "OCS/Utilities/PackedArray.hpp"
#include "OCS/Misc/NonCopyable.hpp"

namespace ocs
{

template<typename T>
using messagePtr = std::shared_ptr<T>;

using MessageSlot = std::vector<messagePtr<BaseMessage>>;
using MessageBoard = std::unordered_map<Family, MessageSlot>;

/** \brief Handles the posting and retrieving of messages. Users called "Transceivers" may create an object
 *         that inheritys from "Message", and post this object for other transceivers to see. These messages
 *         should be cleared periodically.
 *         Registered Transceivers also receive a message slot that other transceivers can send messages to.
 *         These messages stay active until the recipient transceiver views these messages.
 *
 *         Messages are sorted by their type. There is no need for a transceiver to subscribe to a certain message
 *         type. They can simply get a list of the desired message type from the message board.
 *
 *
 *  \author Kevin Miller
 *  \version 2-22-2014
 */
class MessageHub : NonCopyable
{
    public:

        //!Post a message that is available to all users.
        template<typename T, typename ... Args>
        void postMessage(const Transceiver&, Args&& ...);

        //!Get a list of a specific type of message from the message board. Does not delete messages.
        template<typename T>
        std::vector<messagePtr<T>> readPostedMessages();

        /*!Send a message directly to a transceiver that only the recipient can view.
        This message is available until the recipient views it.*/
        template<typename T, typename ... Args>
        void sendPrivateMessage(ID receiverID, const Transceiver&, Args&& ...);

        //!Get a list of the specified message type from the personal message slot. Deletes the private messages.
        template<typename T>
        std::vector<messagePtr<T>> readPrivateMessages(const Transceiver&);

        //!Log all messages that are currently posted on the message board to the specified stream.
        void logPostedMessages(std::ostream&);

        //!Log all private messages for the given transceiver to the specified stream.
        void logPrivateMessages(const Transceiver&, std::ostream&);

        //!Clear all messages from the message board.
        void clearPostedMessages();

        //!Clear all messages from the message board.
        void clearPrivateMessages(const Transceiver&);

    protected:

        //!Get a new Transceiver id.
        static ID getNewTransceiverID();

    private:

        friend class Transceiver;

        //!Clear a message slot.
        void clearMessageSlot(MessageSlot&);

        //!Log a message to a given messageboard.
        void logMessages(const MessageBoard&, std::ostream&);

        //!Where all messages are posted.
        MessageBoard messageBoard;

        //!A list of private message boards.
        std::unordered_map<ID, MessageBoard> privateMessages;

        static ID transceiverIdCounter;
};

template<typename T, typename ... Args>
void MessageHub::postMessage(const Transceiver& transceiver, Args&& ... args)
{
    messagePtr<BaseMessage> msg(new T(transceiver, std::forward<Args>(args)...));

    messageBoard[T::getFamily()].push_back(msg);
}

template<typename T>
std::vector<messagePtr<T>> MessageHub::readPostedMessages()
{
    std::vector<messagePtr<T>> msgSlot;

    for(auto& baseMsg : messageBoard[T::getFamily()])
    {
        auto msg = std::static_pointer_cast<T>(baseMsg);
        msgSlot.push_back(msg);
    }

    return msgSlot;
}

template<typename T, typename ... Args>
void MessageHub::sendPrivateMessage(ID receiverID, const Transceiver& transceiver, Args&& ... args)
{
    messagePtr<BaseMessage> msg( new T(transceiver, std::forward<Args>(args)...) );

    privateMessages[receiverID][T::getFamily()].push_back(msg);
}

template<typename T>
std::vector<messagePtr<T>> MessageHub::readPrivateMessages(const Transceiver& transceiver)
{
    std::vector<messagePtr<T>> messages;

    auto& privateMsgSlot = privateMessages[transceiver.getID()][T::getFamily()];
    for(auto& msg : privateMsgSlot)
    {
        messagePtr<T> message = std::static_pointer_cast<T>(msg);

        if(message)
            messages.push_back(message);
    }

    return messages;
}

}//ocs

#endif
