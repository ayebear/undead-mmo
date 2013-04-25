/*
MINI ENTITY LISTS (both client and server)
EntityList class:
    Use an std::map<EntityId,Entity*> for partial entity lists (on client/in grid on server).
Purpose:
    Client: Main entity list
    Server: Used for each cell in the grid
Caveats:
    Requires an expensive search to access an entity pointer from an ID.
    Has more memory overhead than a simple array.
This will automatically be sorted and therefore very efficient.
Access time is still slower than a basic array or vector.
The client will only need to instantiate 1 EntityList object, which can hold everything the server sends to it.
    This is much simpler than having a mini grid or something, but will still be efficient,
    and that will be the only thing the client will have to do (rather than an entire grid)
*/

#ifndef ENTITYLIST_H
#define ENTITYLIST_H

// Will need some of these
#include <vector>
#include <deque>
#include <map>
#include <list>
#include "entity.h"

class EntityList
{
    public:

        static std::map<EntityId,Entity*> ents; // move this to private
    private:

};

#endif
