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

#ifndef _SYSTEMMANAGER_H
#define _SYSTEMMANAGER_H

#include <list>
#include <queue>
#include <map>

#include "OCS/Systems/System.hpp"
#include "OCS/Misc/Config.hpp"
#include "OCS/Misc/NonCopyable.hpp"

namespace ocs
{

class ObjectManager;
class MessageHub;

/** \brief Manages the updating of a list of systems. Systems should be added in the order that they should be run in.
 *         To create a system, users should inherit from the "System" object and implement an update function. Systems
 *         will have access to an ObjectManager and a MessageHub.
 *
 * \author Kevin Miller
 * \version 2-22-2014
 *
 */
class SystemManager : NonCopyable
{
    public:

        SystemManager(ObjectManager&, MessageHub&);
        ~SystemManager();

        /*!Add a system to the list of systems. Systems will be updated in the order they are added in.
           There can be one instance of each type of system*/
        template<typename T>
        void addSystem();

        //!Remove a system from the list.
        template<typename T>
        void removeSystem();

        //!Query a specific system
        template<typename T>
        bool hasSystem() const;

        //!update a the system of the specified type. This can be used to update systems out of order.
        template<typename T>
        void updateSystem(double);

        //!Get the total number of systems.
        ID getTotalSystems() const;

        //!Get the SystemManager's version number
        ID getVersion() const;

        //!Update all systems in the list in order.
        void updateAllSystems(double);

    private:

        ObjectManager& objManager;
        MessageHub& msgHub;

        std::list<systemPtr<System>> systemList;

        //!Used to get a system by type.
        template<typename T>
        systemPtr<T>& system(systemPtr<T> systemPointer = systemPtr<T>()) const;

        //Used to manage the static pointers for different instance of the same system types
        ID version;
        static ID versionCounter;
        static std::queue<ID> availableVersions;
};

/** \brief Add a new system to the manager.
 *
 */
template<typename T>
void SystemManager::addSystem()
{
    if(!system<T>())
    {
        //Create the system
        systemPtr<T> _system(new T());

        //Store a pointer to the system.
        system(_system);

        //Add the system to the list
        systemList.push_back(std::static_pointer_cast<System>(_system));
    }
}

/** \brief Removes the specified system from the manager.
 */
template<typename T>
void SystemManager::removeSystem()
{
    if(system<T>())
    {
        systemList.remove(std::static_pointer_cast<System>(system<T>()));
        system<T>().reset();
    }
}

/** \brief Check if the SystemManager has the specified system.
 */
template<typename T>
bool SystemManager::hasSystem() const
{
    if(system<T>())
        return true;
    return false;
}

/** \brief Manually update a system
 *
 * \param dt The time elapsed since the last frame
 */
template<typename T>
void SystemManager::updateSystem(double dt)
{
    if(system<T>())
        system<T>()->update(objManager, msgHub, dt);
}

/** \brief Stores a pointer to a system in memory. If a value is given to the function,
 *         the system pointer inside will be changed to point to the new value.
 *
 * \param systemPointer(optional) A shared pointer to the system
 *
 * \return The current system that the pointer holds.
 *
 */
template<typename T>
systemPtr<T>& SystemManager::system(systemPtr<T> systemPointer) const
{
    static std::map<ID, systemPtr<T> > _system;

    if(systemPointer != systemPtr<T>())
        _system[version] = systemPointer;

    return _system[version];
}

}//ocs

#endif
