// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entitylist.h"

using namespace std;

EntityList::EntityList()
{
}

void EntityList::Insert(Entity* newEnt)
{
    ents[newEnt->GetID()] = newEnt;
}

void EntityList::Insert(Entity* newEnt, EID id)
{
    ents[id] = newEnt;
}

Entity* EntityList::Find(EID id)
{
    auto i = ents.find(id);
    if (i == ents.end())
        return nullptr;
    else
        return i->second;
}

void EntityList::Delete(EID id)
{
    ents.erase(id);
}
