#ifndef ENTITY_H
#define ENTITY_H

#include <deque>
#include <SFML/Graphics.hpp>
#include "other.h"

using namespace std;

class Entity// : public sf::Drawable
{
    public:
        Entity();
        void SetTexture(sf::Texture&);
        void SetPos(sf::Vector2f&);
        void SetPos(float, float);
        void UpdateLists(); // Goes through Init list and adds them to the entityList. Goes through clean list and removes them from the entity list.

        //virtual void draw(sf::RenderTarget& target, RenderStates states) const;

        sf::Sprite sprite;

        static uint IDCounter;

    protected:
        uint ID; // TODO: make an ID system, and make this get an open ID from it
        // This ID system MUST save a log of around 100 recently used IDs.
        // The reason for this is what if a new entity is spawned with the same ID as one that was just killed.
        // The server doesn't manage what the client knows, it just sends everything that changes to all of the clients.
        // So, this could cause a conflict, where the client(s) think that entity still exists, and uses those old values.
        // Now, it would probably update it all correctly, but still, it could cause issues.
        // We could simply have a deque or linked list with all of the old IDs as part of the ID managment class/function.

        bool alive; //This is set to false until an entity is moved from the addList to the entityList.
            //When an entity dies or is set to be deleted. It is set to false again. Basically makes an entity invulnerable
            //for a frame, so that a non-existence error does not occur should it be killed before it makes it onto the entity list.

        static uint GetOpenId();
        sf::Vector2f pos;

    private:
        //Init list is the list which entities are for initialized to. This is so that entities created
        //due to a process occuring in EntityList(idk what, maybe a super zombie that spawns other zombies)
        //will be added to the entitylist at the end of the run through. Don't want to mutate lists when they are running.
        // This will occur all the time when we receive network data, which updates the entity list, because it is running on a separate thread.
        //static deque<Entity*> InitList; // Entities to be added will be temporarily stored on here. Once they are fully created, they will be moved to the main list.
        static deque<Entity*> EntityList; // The main entity list. Contains all of the current entities.
        //static deque<Entity*> CleanList; // Any entities that die or need to be deleted from entity list are pushed on here.

};

#endif // ENTITY_H
