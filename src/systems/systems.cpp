#include "systems.h"

#include <OCS/Objects.hpp>
#include <OCS/Messaging.hpp>

#include "components.h"

void MovementSystem::update(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{
    for (auto& vel : objManager.getComponentArray<Velocity>())
    {
        auto pos = objManager.getComponent<Position>(vel.getOwnerID());

        // If position exists
        if (pos)
        {
            pos->x += vel.dx;
            pos->y += vel.dy;
        }
    }
}




/***********Delete**********
struct Object
{
    Object () : objectID(-1) {}
    ID getObjectID() const { return objectID; }

    void serialize();
    void deSerialize

    protected:

        friend class ObjectManager;
        ID objectID;

        //Used for copying and destroying an object
        --> std::unordered_map<Family, BaseComponentArray*> componentArrays;
        std::unordered_map<Family, ID> componentIndices; <--

};

template<typename C>
ComponentArray<C>& ObjectManager::getComponentArray() const
{
    static std::unordered_map<ID, ComponentArray<C>> components;

    return components[version];
}

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

objManager.bindStringToComponent<Position>("Position");
objManager.bindStringToComponent<RigidBody>("RigidBody");
objManager.bindStringToComponent<Collidable>("Collidable");
objManager.bindStringToComponent<Paddle>("Paddle");
objManager.bindStringToComponent<Velocity>("Velocity");
objManager.bindStringToComponent<Renderable>("Renderable");
objManager.bindStringToComponent<Damager>("Damager");
objManager.bindStringToComponent<Health>("Health");

************************************/





void CollisionSystem::handleCollisions(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{

}

void CollisionSystem::update(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{

}

void RenderingSystem::update(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{

}

void NetworkSystem::update(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{

}

void InfectionSystem::update(ocs::ObjectManager& objManager, ocs::MessageHub& msgHub, double dt)
{

}
