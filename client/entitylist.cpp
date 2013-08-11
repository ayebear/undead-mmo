// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entitylist.h"
#include <iostream>
#include "entityalloc.h"

using namespace std;

EntityList::EntityList()
{
    // TODO: Move this elsewhere, maybe in some client-specific entity allocator
    // Load character textures
    if (!textures[0].loadFromFile("data/images/characters/character.png"))
        exit(101);
    textures[0].setSmooth(true);
    if (!textures[1].loadFromFile("data/images/characters/zombie.png"))
        exit(102);
    textures[1].setSmooth(true);
}


void EntityList::updateEntity(EID id, sf::Packet& packet)
{
    Entity* ent = find(id);
    EType type;
    packet >> type;
    if (ent == nullptr) // If the entity does not exist already
        ent = addWithTexture(type, id); // Add a new default entity of that type to the list with that ID

    if (ent != nullptr) // If the entity exists
    {
        // TODO: We should find a better way to destroy entities on the client...
        // It would be best if each client could keep track of it to lessen the load on the server.
        // Maybe some kind of timeout or out of range of the client's entity area.
        if (type == -1)
        {
            erase(id); // Delete the entity
            cout << "Deleted entity " << id << endl;
        }
        else
        {
            if (ent->getType() == type)
            {
                ent->setData(packet); // Update the entity with data from the packet
                //cout << "Set entity " << ent->getID() << " with data from packet.\n";
            }
            else
            {
                //cout << "ERROR: Non-matching entity type update received!\n";
                // Recreate the entity with a new type
                erase(id);
                addWithTexture(type, id);
                cout << "Recreated entity " << id << " with type " << type << endl;
            }
        }
    }
    else
        cerr << "ERROR: Problem allocating new entity in updateEntity()!\n";
}

Entity* EntityList::addWithTexture(EType type, EID id)
{
    Entity* ent = add(type, id);
    if (ent != nullptr)
    {
        if (type >= 0 && type <= 1)
            ent->setTexture(textures[type]);
        else
            cerr << "ERROR: Missing texture for entity type " << type << endl;
    }
    return ent;
}

// This function allocates a new entity based on type AND inserts it into the entity list
Entity* EntityList::add(EType type, EID id)
{
    return insert(allocateEntity(type), id);
}

Entity* EntityList::insert(Entity* newEnt, EID id)
{
    if (newEnt != nullptr) // Don't add a null pointer to the list!
    {
        newEnt->setID(id); // Set its ID
        ents[id] = newEnt; // Store it in the list
    }
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

void EntityList::erase(EID id)
{
    delete ents[id]; // Deallocate
    ents.erase(id); // Remove the pointer
}

void EntityList::clear()
{
    for (auto& ent: ents) // Go through the entities
        delete ent.second; // Deallocate them
    ents.clear(); // Remove all pointers
    cout << "Cleared entity list.\n";
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
