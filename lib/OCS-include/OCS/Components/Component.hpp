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

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "OCS/Misc/Config.hpp"
#include "OCS/Utilities/Serializer.hpp"

namespace ocs
{

class ObjectManager;

/*! \brief Components should not inherit from this struct
*          Please see Component<Derived>
*
*          BaseComponent is used internally by the Component template struct to keep track of
*          registered component IDs
*
*/
struct BaseComponent
{
    virtual ~BaseComponent() {}

    ID getOwnerID() const { return ownerID; }

    protected:

        friend class ObjectManager;

        ID ownerID;
        static Family familyCounter;
};

/*! \brief All created components should inherit from this in the following way:
 *
 *         struct Position : public Component<Position>
 *
 * Derived components may implement serialize and deSerialize functions if the user wishes to load them
 * from a file or pass the component's information in a string.
 *
 * All derived components MUST implement a default constructor and SHOULD implement a paramaterized constructor.
 *
 */
template<typename Derived>
struct Component : public BaseComponent
{
    virtual ~Component() {}
    virtual std::string serialize() { return (""); }
    virtual void deSerialize(const std::string&) {}

    /*! \brief Assigns a family id if the component does not already have one.
    *          Returns the family id for the given component
    *
    *   \return The family id of the component.
    */
    static Family getFamily()
    {
        static Family family = BaseComponent::familyCounter++;
        return family;
    }

    protected:
        Serializer serializer;
};

}//ocs

#endif
