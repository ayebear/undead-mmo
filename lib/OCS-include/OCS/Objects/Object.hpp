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

#ifndef _OBJECT_H
#define _OBJECT_H

#include <unordered_map>

#include <OCS/Misc/Config.hpp>
#include <OCS/Components/ComponentArray.hpp>

namespace ocs
{

class ObjectManager;

/*!
*   \brief Represents a single game object identified by a unique ID.
*          Every object has a set of components that identify how it
*          interacts with other game objects.
*/
struct Object
{
    Object () : objectID(-1) {}
    ID getObjectID() const { return objectID; }

    protected:

        friend class ObjectManager;
        ID objectID;

        //Used for copying and destroying an object
        std::unordered_map<Family, BaseComponentArray*> componentArrays;
        std::unordered_map<Family, ID> componentIndices;
};

}//ocs

#endif
