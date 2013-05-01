// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entitylist.h"
#include "../shared/entityalloc.h"

using namespace std;

EntityList::EntityList()
{
}

void EntityList::Add(int type, EID id)
{
    Insert(AllocateEntity(type), id);
}

// Probably won't need this overload
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
    delete ents[id]; // Deallocate
    ents.erase(id); // Remove the pointer
}

void EntityList::Update(float time)
{
    for (auto& ent: ents)
        ent.second->Update(time);
}

void EntityList::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    for (auto& ent: ents)
        window.draw(*(ent.second), states);
}
