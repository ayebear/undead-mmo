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

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <map>
#include <queue>
#include <utility>

#include <OCS/Components/Component.hpp>
#include <OCS/Misc/NonCopyable.hpp>
#include <OCS/Objects/Object.hpp>
#include <OCS/Components/ComponentArray.hpp>
#include <OCS/Misc/Config.hpp>

namespace ocs
{

//!Used to end the recursive calls that use variadic templates.
struct SentinalType : public Component<SentinalType> {};

/**\brief Manages the lifetime of game objects. A blank object can be created
*         and components may be added manually, or alternatively, the user
*         may specify a custom prototype to copy the new object from.
*
* This class relies heavily on templates and static variables to achieve an effect where any type
* of component can be created and added to an object without the need to hard program
* anything is the manager.
* The object's themselves consist of an ID(their index in objects array),
* whether or not the object is a prototype, and two maps that store indicators of the object's
* components. These maps are called upon when destroying an object and copying a prototype to
* create a new object.
*
*@author Kevin Miller
*@version 2-22-2014
*/
class ObjectManager : NonCopyable
{
    public:

        //!Default constructor
        ObjectManager();
        ~ObjectManager();

        //!Add a component from an exisiting component.
        template<typename C, typename ... Args>
        ID addComponents(ID, const C&, Args&& ...);

        //!Add the given components to an object prototype under the specified name.
        template<typename C, typename ... Args>
        void addComponentsToPrototype(const std::string&, const C&, Args&& ...);

        //!Add components to a prototype using strings. Used mainly for prototype file loading.
        void addComponentToPrototypeFromString(const std::string&, const std::string&, const std::string&);

        //!Allow a component to be referred to by a string
        template<typename C>
        void bindStringToComponent(const std::string&);

        //!Create a game object with no components.
        ID createObject();

        //!Create a game object from an object prototype.
        ID createObject(const std::string&);

        //!Create a game object from an object prototype (Overloaded to differentiate from template function)
        ID createObject(const char* str) { return createObject(std::string(str)); }

        //!Create a game object from one or more components.
        template<typename C,typename ... Args>
        ID createObject(const C&, Args&& ...);

        //!Destroy a game object from the object's ID.
        void destroyObject(ID);

        //!Destroy all game objects.
        void destroyAllObjects();

        //!Check if a prototype of the specified name exists.
        bool doesPrototypeExist(const std::string&) const;

        //!Get a single component from the object's ID.
        template<typename C>
        C* const getComponent(ID);

        //!Retrives the given component's array. If it does not exists, one is created and returned.
        template<typename C>
        ComponentArray<C>& getComponentArray() const;

        //!Get a count of the specidifed component
        template<typename C>
        ID getTotalComponents() const;

        //!Get a count of all objects
        ID getTotalObjects() const;

        //!Check if an object has the specified prototype.
        template<typename C = SentinalType, typename ... Args>
        bool hasComponents(ID);

        //!Check if an object id is a prototype's id.
        bool isPrototype(ID);

        //!Remove a component from the object's ID
        template<typename C = SentinalType, typename ... Args>
        ID removeComponents(ID);

        //!Remove the given components from an object prototype under the specified name.
        template<typename C>
        void removeComponentFromPrototype(const std::string&);

        //!Set a component from an existing component.
        template<typename C>
        void setComponent(ID, const C&);

        //!Set a component through the component's constructor arguments.
        template<typename C, typename ... Args>
        void setComponent(ID, Args&& ...);

    private:

        //!All game objects reside in here.
        PackedArray<Object> objects;

        //!Stores an object prototype under its name for easy lookup.
        std::unordered_map<std::string, Object> objectPrototypes;

        //!Stores a prototype base component array with an associated id
        std::unordered_map<ID, BaseComponentArray*> compFamilyToProtoCompArray;

        //!Stores a prototype base component array with an associated id
        std::unordered_map<ID, BaseComponentArray*> compFamilyToCompArray;

        //!Stores a component id with an associated string
        std::unordered_map<std::string, ID> stringToCompFamily;

        //!Stores components for object prototypes
        template<typename C>
        ComponentArray<C>& getPrototypeComponentArray() const;

        //!Overload function with an empty template paramater list to allow recursion
        ID addComponents(ID) { return 0;}

        //!Overload function with an empty template paramater list to allow recursion
        void addComponentsToPrototype(const std::string&) { }

        //!Called on component creation
        template<typename C>
        void registerComponent();

        template<typename T>
        bool endRecursion(const T&) { return false; }
        bool endRecursion(const SentinalType&) { return true; }

        static ID prototypeIDCounter;

        //Used internally, so different instances of the ObjectManager can have different component arrays
        ID version;
        static ID versionCounter;
        static std::queue<ID> availableVersions;
};

/** \brief Return a reference to an array of the specified component.
 *
 * \return A reference to a component's array.
 */
template<typename C>
ComponentArray<C>& ObjectManager::getComponentArray() const
{
    static std::unordered_map<ID, ComponentArray<C>> components;

    return components[version];
}

/** \brief Return a reference to the arrays for prototype components.
 *
 * \return A reference to the prototype component array
 */
template<typename C>
ComponentArray<C>& ObjectManager::getPrototypeComponentArray() const
{
    static std::unordered_map<ID, ComponentArray<C>> components;

    return components[version];
}

/** \brief Create an object from one or more components. Only one instance of each component will be added to the object.
 *
 * \param component The first component to be added.
 * \param others The other components to be added.
 * \return The created object's id.
 *
 */
template<typename C, typename ... Args>
ID ObjectManager::createObject(const C& component, Args&& ... others)
{
    ID id = createObject();
    addComponents(id, component, others...);

    return id;
}

/** \brief Add the given components to the object with the specified id. Each object can only have one instance of each component.
 *
 * \param objectID The id of the object to add the components to.
 * \param component The first component to add.
 * \param others Any other components to add.
 * \return The total number of components that were added.
 *
 */
template<typename C, typename ... Args>
ID ObjectManager::addComponents(ID objectID, const C& component, Args&& ... others)
{
    registerComponent<C>();

    //A counter for the total components added to the object
    ID added = 0;

    if(objectID < getTotalObjects())
    {
        //Only add the component if the object does not have an instance of it already.
        if(objects[objectID].componentIndices.find(C::getFamily()) == objects[objectID].componentIndices.end())
        {
            //Add the component to its array

            ID componentIndex = getComponentArray<C>().add_item(component);
            getComponentArray<C>()[componentIndex].ownerID = objectID;

            //Store a pointer to the components PackedArray for destroying of the object
            objects[objectID].componentArrays[C::getFamily()] = &getComponentArray<C>();

            //Store the component's index in the object
            objects[objectID].componentIndices[C::getFamily()] = componentIndex;

            added = 1;
        }
    }

    //If there are more objects to add then add them.
    if(sizeof ... (others) > 0)
        return added + addComponents(objectID, others...);

    return added;
}

/** \brief Set the value of a component to the given value.
 *
 * \param objectID The owner object's id.
 * \param value The new value for the component.
 *
 */
template<typename C>
void ObjectManager::setComponent(ID objectID, const C& value)
{
    if(objectID < getTotalObjects())
    {
        auto compPtr = getComponent<C>(objectID);
        // If the object has the specified component
        if(compPtr)
        {
            //Set the objects component to the new value
            *compPtr = value;
            return true;
        }
    }

    return false;
}

/** \brief Set a component's value from the component's constructor arguments
 *
 * \param objectID The owner object's id.
 * \param args The component's constructor arguments.
 *
 */
template<typename C, typename ... Args>
void ObjectManager::setComponent(ID objectID, Args&& ... args)
{
    C newValue(std::forward<Args>(args)...);
    setComponent<C>(objectID, newValue);
}

/** \brief Remove the specified components from object with the given id.
 *
 * \param objectID The owner object's id.
 * \return True if the component was removed. False if it was not removed.
 *
 */
template<typename C, typename ... Args>
ID ObjectManager::removeComponents(ID objectID)
{
    ID totalRemoved = 0;
    if(!endRecursion(C()))
    {
        if(objectID < getTotalObjects())
        {
            //Only remove the component if the object has an instance of it.
            if(objects[objectID].componentIndices.find(C::getFamily()) != objects[objectID].componentIndices.end())
            {
                ID componentIndex = objects[objectID].componentIndices[C::getFamily()];

                //Remove the component from its array
                getComponentArray<C>().remove(componentIndex);

                //Remove the pointer to the component maps from the object
                objects[objectID].componentArrays.erase(C::getFamily());
                objects[objectID].componentIndices.erase(C::getFamily());

                totalRemoved = 1;
            }

            return totalRemoved + removeComponents<Args...>(objectID);
        }
    }

    return totalRemoved;
}

/** \brief Associate a string with a component
 *
 * \param compName The string to register
 */
template<typename C>
void ObjectManager::bindStringToComponent(const std::string& compName)
{
    //Only bind the string if it is not already in use.
    if(stringToCompFamily.find(compName) == stringToCompFamily.end())
    {
        stringToCompFamily[compName] = C::getFamily();
        registerComponent<C>();
    }
}

//!Called on component creation
template<typename C>
void ObjectManager::registerComponent()
{
    if(compFamilyToCompArray.find(C::getFamily()) == compFamilyToCompArray.end())
    {
        compFamilyToProtoCompArray[C::getFamily()] = &getPrototypeComponentArray<C>();
        compFamilyToCompArray[C::getFamily()] = &getComponentArray<C>();
    }
}

/** \brief Get the total number of the specified component.
 *
 * \return The total number of the specified component.
 *
 */
template<typename C>
ID ObjectManager::getTotalComponents() const
{
    return getComponentArray<C>().size();
}

/** \brief Get a pointer to the specified component that belongs to the object with the given id.
 *
 *         WARNING: Pointer will be invalidated if the component array is resized
 *         Do not store the pointer, and do not add more components of the same
 *         type before using the pointer
 *
 *         If the object does not have the specified component, the returned pointer
 *         will be null. Please check for this before using.
 *
 *         To keep track of an object, store the ID and call this function every update.
 *
 * \param objectID The owner object's id.
 * \return If the object has an instance of the specified component, returns a pointer to the component. If
 *         the object does not have an instance, return a nullptr.
 *
 */
template<typename C>
C* const ObjectManager::getComponent(ID objectID)
{
    C* compPtr = nullptr;

    if(objects.isValid(objectID))
    {
        //If the object has the specified component
        if(objects[objectID].componentIndices.find(C::getFamily()) != objects[objectID].componentIndices.end())
        {
            ID componentIndex = objects[objectID].componentIndices[C::getFamily()];

            compPtr = &getComponentArray<C>()[componentIndex];
        }
    }
    return compPtr;
}

/** \brief Add components to the prototype that has the given name.
 *         If there is no existing prototype under the given name, one is created.
 *         A prototype may only have one instance of each type of component.
 *
 * \param prototypeName The name of the prototype to add the components to.
 * \param first The first component to add to the prototype.
 * \param others Any other components to add to the prototype.
 *
 */
template<typename C, typename ... Args>
void ObjectManager::addComponentsToPrototype(const std::string& prototypeName, const C& first, Args&& ... others)
{
    registerComponent<C>();

    //Get the prototype
    auto& prototype = objectPrototypes[prototypeName];

    if(prototype.objectID == ID(-1))
        prototype.objectID = prototypeIDCounter++;

    //If the prototype does not already have the given component
    if(prototype.componentArrays.find(C::getFamily()) == prototype.componentArrays.end())
    {
        //Add the first component in the list
        auto compIdx = getPrototypeComponentArray<C>().add_item(first);

        prototype.componentArrays[C::getFamily()] = &getPrototypeComponentArray<C>();
        prototype.componentIndices[C::getFamily()] = compIdx;

        //If there are more components to add, keep adding them
        if(sizeof...(others) > 0)
            addComponentsToPrototype(prototypeName, others...);
    }

}

/** \brief Remove the specified component from the prototype with the given name.
 *
 * \param prototypeName The name of the prototype to remove the component from.
 */
template<typename C>
void ObjectManager::removeComponentFromPrototype(const std::string& prototypeName)
{
    //If the prototype has the specified component
    if(objectPrototypes.find(prototypeName) != objectPrototypes.end())
    {
        auto& compArray = objectPrototypes[prototypeName].componentArrays;
        auto& compIndices = objectPrototypes[prototypeName].componentIndices;

        //If the prototype has the component to remove
        if(compArray.find(C::getFamily()) != compArray.end())
        {
            compArray[C::getFamily()]->remove(compIndices[C::getFamily()]);

            compArray.erase(C::getFamily());
            compIndices.erase(C::getFamily());
        }
    }
}

template<typename C, typename ... Args>
bool ObjectManager::hasComponents(ID objectID)
{
    if(!endRecursion(C()))
    {
        if(getComponent<C>(objectID))
        {
            //Return a pointer to the component converted to a boolean
            return true && hasComponents<Args...>(objectID);
        }
        return false;
    }

    return true;


}

}//ocs

#endif

