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

#include "OCS/Messaging/MessageHub.hpp"

namespace ocs
{

ID MessageHub::transceiverIdCounter = 0;

ID MessageHub::getNewTransceiverID()
{
    return transceiverIdCounter++;
}

void MessageHub::clearPostedMessages()
{
    for(auto& messageSlot : messageBoard)
        clearMessageSlot(messageSlot.second);
}

void MessageHub::clearPrivateMessages(const Transceiver& transceiver)
{
    for(auto& messageSlot : privateMessages[transceiver.getID()])
        clearMessageSlot(messageSlot.second);
}

void MessageHub::logMessages(const MessageBoard& msgBoard, std::ostream& out)
{
    for(const auto& msgTypeArrays : msgBoard)
    {
        for(const auto& msgs : msgTypeArrays.second)
        {
            out << msgs->getTimeStamp() << std::endl;
            msgs->log(out);
            out << std::endl;
        }
    }
}


void MessageHub::logPostedMessages(std::ostream& out)
{
    logMessages(messageBoard, out);
}

void MessageHub::logPrivateMessages(const Transceiver& transceiver, std::ostream& out)
{
    logMessages(privateMessages[transceiver.id], out);
}

void MessageHub::clearMessageSlot(MessageSlot& msgSlot)
{
    msgSlot.clear();
}

}//ocs
