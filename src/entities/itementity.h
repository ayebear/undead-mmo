#ifndef ITEMENTITY_H
#define ITEMENTITY_H

#include "entity.h"

class ItemEntity: public Entity
{
    public:
        ItemEntity();
        void update(float);
        bool collides(Entity*);
        void draw(sf::RenderTarget&, sf::RenderStates) const;
        void getData(sf::Packet&);
        void setData(sf::Packet&);

    private:
        // int itemId; // This should be in the base class
};

#endif
