// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "other.h"

class Entity : public sf::Drawable
{
    public:
        Entity();
        virtual ~Entity();
        void SetTexture(sf::Texture&);
        void SetPos(sf::Vector2f&);
        sf::Vector2f GetPos();
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        static EntityId IDCounter;

    protected:
        static EntityId GetOpenId();
        EntityId ID;
        bool isReady;
        sf::Vector2f pos;
        sf::Sprite sprite;
};

/*
#include <array>
#include "entitylist.h"

class EntityList; //This one is fine, but it annoys me that it won't work without it ...

class Entity
{
public:
    const int* _ID;

    static EntityList* ents;

    Entity();
    virtual ~Entity();

    bool setPosition(std::array<int,2>);//,Map);
    std::array<int,2> getPosition() const;
    void update();
    void setReady(bool);
    bool getReady() const;
    //virtual bool draw();

    bool operator==(const Entity &other) const;
    bool operator<=(const Entity &other) const;
    bool operator>=(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;
private:
    bool isReady;
    std::array<int,2> position;
};
*/
#endif


/* OLD
#ifndef ENTITY_H
#define ENTITY_H

#include <deque>
#include <SFML/Graphics.hpp>
#include "other.h"

using namespace std;

class Entity : public sf::Drawable
{
    public:
        Entity();
        void SetTexture(sf::Texture&);
        void SetPos(sf::Vector2f&);
        void SetPos(float, float);
        sf::Vector2f GetPos();
        void UpdateLists(); // Goes through Init list and adds them to the entityList. Goes through clean list and removes them from the entity list.
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        static bool InsertEntity(Entity*);
        static int FindEntity(Entity*);
        static bool DeleteEntity(Entity*);

        static uint IDCounter;

    protected:
        static uint GetOpenId();

        uint ID; // The entity's ID

        bool isReady; //This is set to false until an entity is moved from the addList to the entityList.
            //When an entity dies or is set to be deleted. It is set to false again. Basically makes an entity invulnerable
            //for a frame, so that a non-existence error does not occur should it be killed before it makes it onto the entity list.

        sf::Vector2f pos;
        sf::Sprite sprite;

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
*/
