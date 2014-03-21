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

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <cstdint>
#include <iostream>
#include <string>

namespace ocs
{

class Transceiver;

using ID = uint64_t;


/** \brief This struct should not be inherited from. It is used internally to keep track of message ids
 *         and for storage in the MessageHub.
 *
 *         To create a message \see Message
 *
 */
struct BaseMessage
{
    using Family = uint64_t;

    BaseMessage(const Transceiver&);

    virtual ~BaseMessage() {}
    virtual void log(std::ostream&) {}

    ID getSender() const { return senderID; }
    std::string getTimeStamp() const { return timeStamp; }

    protected:

        static Family familyCounter;

        ID senderID;
        std::string timeStamp;
};

/** \brief A user defined message can be created by inheriting from "Message" and passing in the type as the template paramater.
 *         e.g.
 *             struct TextMessage : public ocs::Message<TextMessage>
 *             {
 *                 TextMessage(const Transceiver& _transceiver, const std::string& _msg) :
 *                      Message(_transceiver), msg(_msg)
 *                 {
 *
 *                 }
 *                 std::string msg;
 *             }
 *
 *         The message struct can be as complex or as simple as you like. Just simply post it to the MessageHub and other
 *         transceivers that are interested in that type can view it.
 *
 *
 */
template<typename T>
struct Message : public BaseMessage
{
    Message(const Transceiver& _transceiver) : BaseMessage(_transceiver) {}

    virtual ~Message() {}

    virtual void log(std::ostream& out)
    {
        out << "Message Family: " << getFamily() << std::endl;
        out << "Sender: " << getSender() << std::endl;
    }

    static Family getFamily()
    {
        static Family family = BaseMessage::familyCounter++;
        return family;
    }

};

}//ocs


#endif
