// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "map.h"

typedef sf::Uint32 EID;

class Entity: public sf::Drawable
{
    public:
        Entity();
        virtual ~Entity();
        const EID getID() const;
        void setID(EID);

        // These are the functions that all entities will have (Which need to be defined by classes which inherit from Entity)
        virtual void update(float) = 0;
        virtual bool collides(Entity*);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

        // Use a vector of sprites, and each inheriting entity class has an enum
        // Also, all of this code should be able to be cut out on the server side
        // We should be able to get rid of this and have it in the constructor, after we make a resource manager class.
        void setTexture(const sf::Texture&);

        // This will be great for optimizing stuff, and doubly acts as a way to separate dynamic/static entities!
        // It also can be false even with dynamic entities.
        // Note that there is already a bool variable for this in the EntityLiving class. We just need this function in the Entity class to access it.
        virtual bool isMoving();

        // For setting/getting if the entity is ready or not
        void setReady(bool);
        const bool getReady() const;

        virtual sf::Packet getPacket() = 0;
        virtual sf::Packet& setData(sf::Packet&) = 0;

        virtual void setAngle(float) {}
        virtual void setSpeed(float) {}
        virtual void setMoving(bool) {}
        void setPos(const sf::Vector2f&);
        const sf::Vector2f getPos() const;
        virtual void moveTo(const sf::Vector2f&);

        static void setMapSize(int, int);

        // All of the different entity types
        enum Type
        {
            Player = 0,
            Zombie,
            Weapon
        };

    protected:
        // We could also have a mutex for use with threads, but this is good for determining
        // whether the entity is fully initialized and its textures are all set and stuff
        bool ready;
        // Contains the entity's unique ID
        EID ID;
        // Represents what type the entity is
        int type;
        sf::Vector2f pos;
        sf::Sprite sprite;

        static int mapWidth;
        static int mapHeight;

};

sf::Packet& operator<<(sf::Packet&, const Entity&);

sf::Packet& operator>>(sf::Packet&, Entity&);

#endif
