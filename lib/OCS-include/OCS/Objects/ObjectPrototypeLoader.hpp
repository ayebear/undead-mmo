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

#ifndef _OBJECTPROTOTYPELOADER_H
#define _OBJECTPROTOTYPELOADER_H

#include <string>

#include "OCS/Objects/ObjectManager.hpp"


namespace ocs
{

/** \brief Handles the loading of object prototypes from a file. User can load individual prototypes
 *         or multiple prototypes at once. The object's components should be formatted in a way that
 *         can be converted using the component's deSerialize function.
 */
class ObjectPrototypeLoader
{
    public:
        //!Load a single object prototype from a file.
        static bool loadObjectPrototype(ObjectManager&, const std::string&, const std::string&, const std::string& = "");

        //!Load multiple object prototypes from a file.
        static int loadPrototypeSet(ObjectManager&, const std::string&, const std::string&, const std::string& = "");

        static bool doesPrototypeExist(const ObjectManager&, const std::string&);
};

}//ocs

#endif

