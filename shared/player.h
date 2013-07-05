// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PLAYER_H
#define PLAYER_H

#include "entityliving.h"

class Player: public EntityLiving
{
    public:
        Player();
        void update(float);
        bool collides(Entity*);
        void draw(sf::RenderTarget&, sf::RenderStates) const;
        sf::Packet getPacket();
        sf::Packet& setData(sf::Packet&);
    private:

};

#endif
