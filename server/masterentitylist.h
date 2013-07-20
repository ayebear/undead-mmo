// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
MASTER ENTITY LIST (server only)
MasterEntityList class:
    Use an std::vector<Entity*> for the main entity list, which stores ALL entities.
        In addition to this, it should also store the entity's grid location, if we ever need to do that.
    Use an std::list to store free IDs. (Only need efficient front/back/pop_front/push_back)
Purpose:
    To manage the IDs of all of the entities. (Efficiently assigns new IDs by recycling them)
    Also allows for instant access to entity pointers from entity IDs.
Caveats:
    This list must only be used for all entities. This is because the entire range of IDs must have allocated space.
When a new entity is added, it will first try to get an ID from the front of the list/deque.
    If this list is empty, it will generate a new one, 1 larger than the largest ID.
    The IDs must be pulled from the front, so IDs are rotated. Otherwise the main list will basically leak memory.
When an entity is deleted, it will simply set the element to a null pointer.
    The ID will be pushed onto the back of the free ID list/deque.
This system is the most efficient so far, although it could possibly accumalate a lot of memory.
    We could have a cleaning function that rebuilds the whole list and all of the entity IDs.
Inserting and deleting entities is mostly just a single assignment operation.
This would be a lot more thread safe than using another container, as memory isn't ever moved around or reallocated.
*/

#ifndef MASTERENTITYLIST_H
#define MASTERENTITYLIST_H

#include "entity.h"
#include <vector>
#include <list>

class MasterEntityList
{
    public:
        MasterEntityList();
        Entity* add(int);
        Entity* insert(Entity*);
        Entity* find(EID);
        void erase(EID);
        bool cleanUp();
        void update(float);
    private:
        static unsigned int entCount;
        static const int cleanUpRatio;
        std::vector <Entity*> ents; // all of the entity pointers are stored here, and accessed by ID directly
        std::list <EID> freeList; // unused IDs go here
};

#endif
