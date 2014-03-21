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

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "OCS/Misc/NonCopyable.hpp"
#include "OCS/Messaging/Transceiver.hpp"

namespace ocs
{

class ObjectManager;
class MessageHub;

/** \brief Base class for user defined logic systems. All systems will have access to an ObjectManager and a MessageHub.
 *         Systems should operate on a set of components, and can look for posted messages of a desired type from the MessageHub.
 */
struct System : NonCopyable, public Transceiver
{
    virtual ~System() {}
    virtual void update(ObjectManager&, MessageHub&, double) = 0;
};

}//ocs

#endif
