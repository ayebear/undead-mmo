// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
EntityList class:
    Will have a std::map<EID,Entity*> for its main data structure.
Purpose:
    Manages the client's main entity list, which stores all of the entity pointers.
    Also manages dynamic memory allocation
The client will only need to instantiate 1 EntityList object, which can hold everything the server sends to it.
    This is much simpler than having a mini grid or something, but will still be efficient,
    and that will be the only thing the client will have to do (rather than process an entire grid).
*/

#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <map>
#include "entity.h"

class EntityList: public sf::Drawable
{
    public:
        EntityList();
        void updateEntity(EID, sf::Packet&);
        Entity* addWithTexture(sf::Int32, EID);
        Entity* add(EType, EID);
        Entity* insert(Entity*, EID);
        Entity* find(EID);
        void erase(EID);
        void clear();
        void update(float);
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        std::map<EID,Entity*> ents; // stores entity pointers, accessed by searching for ID
        sf::Texture textures[2];
};

#endif
