// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "other.h"

/*
TODO:
    May need a type variable to determine what type the entity is.
*/

class Entity : public sf::Drawable
{
    public:
        Entity();
        virtual ~Entity();
        const EID GetID();
        void SetID(EID);

        // These are the functions that all entities will have
        virtual void update(float) = 0;
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
        // TODO: May need to separate dynamic and static entities (don't need to check collision every frame on non-moving entities)
        virtual bool collides(Entity*) = 0;

        // Unsure if entities will have different sprites than just a single one
        // Maybe a vector of sprites, and each inheriting entity class has an enum
        // Also, all of this code should be able to be cut out on the server side
        void SetTexture(const sf::Texture&);
        void SetPos(const sf::Vector2f&);
        sf::Vector2f GetPos();

    protected:
        EID ID;
        bool isReady;
        sf::Vector2f pos;
        sf::Sprite sprite;
};

#endif
