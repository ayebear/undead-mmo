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

#ifndef _STATE_H
#define _STATE_H

#include "OCS/Objects/ObjectManager.hpp"
#include "OCS/Messaging/MessageHub.hpp"
#include "OCS/Systems/SystemManager.hpp"

#include "OCS/Utilities/Timer.hpp"

namespace ocs
{

class State : public Transceiver
{
    public:

        State();
        virtual ~State();

        void run();
        void stop();

        virtual void configure() = 0;
        virtual void initialize() = 0;
        virtual void update(double) = 0;

    protected:

        ObjectManager objManager;
        SystemManager sysManager;
        MessageHub msgHub;

        Timer timer;

        bool running;

};

}//ocs

#endif
