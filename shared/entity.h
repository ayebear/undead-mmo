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

class Entity : public sf::Drawable
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
        void SetPos(const sf::Vector2f&);
        sf::Vector2f GetPos();

    protected:
        // We could also have a mutex for use with threads, but this is good for determining
        // whether the entity is fully initialized and its textures are all set and stuff
        bool isReady;
        EID ID;
        sf::Vector2f pos;
        sf::Sprite sprite;
};

#endif
