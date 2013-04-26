#include "entitylist.h"

using namespace std;

EntityList::EntityList()
{
}

void EntityList::Insert(Entity* newEnt)
{
    ents[newEnt->GetID()] = newEnt;
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
