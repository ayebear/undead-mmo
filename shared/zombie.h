// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "entityliving.h"

/*
Zombies will need a "state" so we know what they are supposed to be doing.
    This could be represented with an enum.
    Within the update function, it will do different things depending on its state and stuff.
*/
class Zombie: public EntityLiving
{
    public:
        Zombie();
        void Update(float);
        bool Collides(Entity*);
        void draw(sf::RenderTarget&, sf::RenderStates) const;
    private:
};

#endif
