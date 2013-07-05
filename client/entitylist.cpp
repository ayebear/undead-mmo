// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entitylist.h"
#include "../shared/entityalloc.h"

using namespace std;

EntityList::EntityList()
{
}

// TODO: We must find a way to destroy entities on the client...
// It would be best if each client could keep track of it to lessen the load on the server.
void EntityList::updateEntity(EID id, sf::Packet& packet)
{
    Entity* ent = find(id);
    int type;
    packet >> type;
    if (ent == nullptr) // If the entity does not exist already
        ent = add(type, id); // Add a new default entity of that type to the list with that ID
    packet >> *ent; // Update the entity with the packet data
    // TODO: Maybe it would be better to just call one of the entity functions and pass in an
    // entire reference to the packet... Like this:
    //ent->setData(packet);
}

// This function allocates a new entity based on type AND inserts it into the entity list
Entity* EntityList::add(int type, EID id)
{
    return insert(allocateEntity(type), id);
}

// Probably won't need this overload
/*void EntityList::insert(Entity* newEnt)
{
    ents[newEnt->getID()] = newEnt;
}*/

Entity* EntityList::insert(Entity* newEnt, EID id)
{
    ents[id] = newEnt;
    return newEnt;
}

Entity* EntityList::find(EID id)
{
    auto i = ents.find(id);
    if (i == ents.end())
        return nullptr;
    else
        return i->second;
}

void EntityList::deleteChar(EID id)
{
    delete ents[id]; // Deallocate
    ents.erase(id); // Remove the pointer
}

void EntityList::update(float time)
{
    for (auto& ent: ents)
        ent.second->update(time);
}

void EntityList::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    for (auto& ent: ents)
        window.draw(*(ent.second), states);
}
