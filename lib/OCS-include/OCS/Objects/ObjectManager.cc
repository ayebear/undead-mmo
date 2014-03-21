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

#include "OCS/Objects/ObjectManager.hpp"

namespace ocs
{

ID ObjectManager::prototypeIDCounter = 0;
ID ObjectManager::versionCounter = 0;
std::queue<ID> ObjectManager::availableVersions;

ObjectManager::ObjectManager()
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

ObjectManager::~ObjectManager()
{
    destroyAllObjects();
    availableVersions.push(version);
}

//Hello
/** \brief Create a blank object
 *
 * \return The id of the created object
 *
 */
ID ObjectManager::createObject()
{
    ID indx = objects.emplace_item();
    objects[indx].objectID = indx;

    return indx;
}

/** \brief Create an object modeled after the prototype of the given name.
 *         If the prototype does not exist, no object is created.
 *
 * \param prototypeName The name of the prototype to base the new object off of.
 *
 * \return If the prototype exists, returns the id of the created object.
 *         If the prototype does not exist, returns -1 converted to an unsigned number
 *
 */
ID ObjectManager::createObject(const std::string& prototypeName)
{

    if(doesPrototypeExist(prototypeName))
    {
        auto& prototype = objectPrototypes[prototypeName];
        ID objectID = createObject();

        //Copy the component families from the prototype
        for(auto& it : prototype.componentArrays)
            objects[objectID].componentArrays[it.first] = compFamilyToCompArray[it.first];

        auto& prototypeCompArrays = prototype.componentArrays;
        auto& componentIndices = objects[objectID].componentIndices;

        //Iterate through all of the objects component array pointers
        for(const auto& it : prototypeCompArrays)
        {
            Family compFamily = it.first;
            auto componentArray = objects[objectID].componentArrays[it.first];

            auto prototypeComponentArray = it.second;

            //Get the index of the prototype's component
            ID prototypeCompIdx = prototype.componentIndices[compFamily];

            //Copy the prototype's component and store the index that it was assigned
            ID newComponentIndex = prototypeComponentArray->createCopy(prototypeCompIdx, componentArray);

            //Make the new object the component's owner
            componentArray->getBaseComponent(newComponentIndex).ownerID = objectID;

            //Store the component's index in the map under the newly created object's ID
            componentIndices[compFamily] = newComponentIndex;

        }

        return objectID;
    }

    std::cerr << "Error: No prototype exists under the name '" << prototypeName << "'\n";
    return -1;
}

/** \brief Removes an object and all of its components from the appropriate arrays.
 *
 * \param objectID The id of the object to be removed.
 *
 */

void ObjectManager::destroyObject(ID objectID)
{
    if(objects.isValid(objectID))
    {
        auto& componentArrays = objects[objectID].componentArrays;

        //Iterate through all of the objects component array pointers
        for(const auto& it : componentArrays)
        {
            Family compFamily = it.first;
            auto componentArray = it.second;

            //Get the component's index
            ID compIndex = objects[objectID].componentIndices[compFamily];

            //Remove the component from the appropriate array
            componentArray->remove(compIndex);
        }
        //Remove the object from the object array
        objects.remove(objectID);
    }
}

/** \brief Destroys all objects by iterating through the objects list and calling destroyObject.
 *
 *  \see destroyObject
 *
 */
void ObjectManager::destroyAllObjects()
{
    if(objects.size() > 0)
    {
        for(ID i = objects.size() - 1; i > 0 ; --i)
            destroyObject(objects[i].objectID);
        destroyObject(0);
    }
}

/** \brief Gets the total number of existing objects.
 *
 *  \return The size of the array containing all objects.
 */
ID ObjectManager::getTotalObjects() const
{
    return objects.size();
}

/** \brief Searches the prototype map for the given prototype name to see if the prototype exists.
 *
 * \param prototypeName The name of the prototype to search for.
 *
 * \return True if the prototype exists. False if it does not.
 *
 */
bool ObjectManager::doesPrototypeExist(const std::string& prototypeName) const
{
    return objectPrototypes.find(prototypeName) != objectPrototypes.end();
}

void ObjectManager::addComponentToPrototypeFromString(const std::string& prototypeName, const std::string& compName, const std::string& compValues)
{
    if(stringToCompFamily.find(compName) != stringToCompFamily.end())
    {
        auto componentFamily = stringToCompFamily[compName];

        //std::cout << "Adding component family: " << componentFamily << std::endl;

        //Get the prototype
        auto& prototype = objectPrototypes[prototypeName];

        if(prototype.objectID == ID(-1))
            prototype.objectID = prototypeIDCounter++;

        //If the prototype does not already have the given component
        if(prototype.componentArrays.find(componentFamily) == prototype.componentArrays.end())
        {
            //Add the first component in the list
            auto compArray = compFamilyToProtoCompArray[componentFamily];

            auto compIdx = compArray->add_item(compValues);

            prototype.componentArrays[componentFamily] = compArray;
            prototype.componentIndices[componentFamily] = compIdx;

        }

          //std::cout << "Finished Adding component family: " << componentFamily << std::endl;
    }
    else
        std::cerr << "Error: " << compName << " not bound to a component\n";

}

}//ocs
