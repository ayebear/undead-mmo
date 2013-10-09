// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "tileset.h"

typedef sf::Int32 EID;
typedef sf::Int32 EType;

// TODO: Redesign this class as well as the whole inheritance tree

class Entity: public sf::Drawable
{
    public:
        Entity();
        virtual ~Entity();
        const EID getID() const;
        void setID(EID);
        EType getType() const;

        // These are the functions that all entities will have (Which need to be defined by classes which inherit from Entity)
        virtual void update(float) = 0;
        virtual bool collides(Entity*);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

        // This will be great for optimizing stuff, and doubly acts as a way to separate dynamic/static entities!
        // It also can be false even with dynamic entities.
        // Note that there is already a bool variable for this in the MobileEntity class. We just need this function in the Entity class to access it.
        virtual bool isMoving() const;

        // For setting/getting if the entity is ready or not
        void setReady(bool);
        bool getReady() const;

        // For setting/getting if the entity state has changed
        void setChanged(bool);
        bool hasChanged() const;

        virtual void getData(sf::Packet&) = 0; // get data from entity into a packet
        virtual void setData(sf::Packet&) = 0; // set data from a packet into entity

        virtual void setAngle(float) {}
        virtual float getVisualAngle() const;
        virtual void setVisualAngle(float) {}
        virtual void setSpeed(float) {}
        virtual void setMoving(bool) {}
        void setPos(const sf::Vector2f&);
        const sf::Vector2f& getPos() const;
        virtual void moveTo(const sf::Vector2f&);

        // Item stuff
        virtual void attachItem(int) {};
        virtual int getItem() const;
        virtual void removeItem() {};
        virtual void useItem() {};

        static void setMapSize(int, int);
        static void loadTextures();

        // All of the different entity types
        enum Type
        {
            Invalid = -1,
            Player = 0,
            Zombie,
            Item
        };

    protected:
        void setTexture(unsigned int);

        static TileSet textures;

        // We could also have a mutex for use with threads, but this is good for determining
        // whether the entity is fully initialized and its textures are all set and stuff
        bool ready;
        bool changed;
        // Contains the entity's unique ID
        EID id;
        // Represents what type the entity is
        EType type;
        sf::Vector2f pos;
        sf::Sprite sprite;

        static int mapWidth;
        static int mapHeight;

};

#endif
