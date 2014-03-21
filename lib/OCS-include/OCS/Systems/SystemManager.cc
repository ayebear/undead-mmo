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

#include "OCS/Systems/SystemManager.hpp"

#include "OCS/Objects/ObjectManager.hpp"

namespace ocs
{

ID SystemManager::versionCounter = 0;
std::queue<ID> SystemManager::availableVersions;

SystemManager::SystemManager(ObjectManager& _objManager, MessageHub& _msgHub) :
    objManager(_objManager),
    msgHub(_msgHub)
{
    if(availableVersions.empty())
    {
        version = versionCounter++;
    }
    else
    {
        version = availableVersions.front();
        availableVersions.pop();
    }
}

SystemManager::~SystemManager()
{
    availableVersions.push(version);
}

void SystemManager::updateAllSystems(double dt)
{
    for(auto& sys : systemList)
        sys->update(objManager, msgHub, dt);
}

ID SystemManager::getTotalSystems() const
{
    return systemList.size();
}

ID SystemManager::getVersion() const
{
    return version;
}

}//ocs
