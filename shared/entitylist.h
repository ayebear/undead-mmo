// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
EntityList class:
    Will have a std::map<EID,Entity*> for its main data structure.
Purpose:
    The client's main entity list
The client will only need to instantiate 1 EntityList object, which can hold everything the server sends to it.
    This is much simpler than having a mini grid or something, but will still be efficient,
    and that will be the only thing the client will have to do (rather than process an entire grid)
*/

#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <map>
#include "entity.h"

class EntityList
{
    public:
        EntityList();
        void Insert(Entity*);
        void Insert(Entity*, EID);
        Entity* Find(EID);
        void Delete(EID);
    private:
        std::map<EID,Entity*> ents; // stores entity pointers, accessed by searching for ID
};

#endif
