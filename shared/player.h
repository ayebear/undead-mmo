// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PLAYER_H
#define PLAYER_H

#include "entityliving.h"

class Player: public EntityLiving
{
    public:
        Player();
        void Update(float);
        bool Collides(Entity*);
        void draw(sf::RenderTarget&, sf::RenderStates) const;
    private:

};

#endif
