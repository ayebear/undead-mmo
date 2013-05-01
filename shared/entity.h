// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "other.h"

/*
TODO:
    May need a type variable to determine what type the entity is.
    May need to separate dynamic and static entities (don't need to check collision every frame on non-moving entities)
*/

class Entity: public sf::Drawable
{
    public:
        Entity();
        virtual ~Entity();
        const EID GetID() const;
        void SetID(EID);

        // These are the functions that all entities will have (Which need to be defined by classes which inherit from Entity)
        virtual void Update(float) = 0;
        virtual bool Collides(Entity*) = 0;
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;

        // Use a vector of sprites, and each inheriting entity class has an enum
        // Also, all of this code should be able to be cut out on the server side
        void SetTexture(const sf::Texture&);

        // This will be great for optimizing stuff, and doubly acts as a way to separate dynamic/static entities!
        // It also can be false even with dynamic entities.
        virtual bool IsMoving() {return false;}

        // TODO: Instead of SetAngle, it needs to set both an angle and a speed. This could be encapsulated in a class/struct.
        virtual void SetAngle(float deg) {}
        void SetPos(const sf::Vector2f&);
        sf::Vector2f GetPos();

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
        bool isReady;
        // Contains the entity's unique ID
        EID ID;
        // Represents what type the entity is
        int type;
        sf::Vector2f pos;
        sf::Sprite sprite;
};

#endif
