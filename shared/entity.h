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
        const EID GetID();
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    protected:
        static EID GetOpenID(); // TODO: move this to the master list
        static EID IDCounter;
        EID ID;
        bool isReady;
        sf::Vector2f pos;
        sf::Sprite sprite;
};

#endif
