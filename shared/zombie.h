// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "entityliving.h"

class Zombie: public EntityLiving
{
    public:
        Zombie();
        void freeRoam (float);
    private:
        static const int defaultSpeed = 300;
        float speed;
        sf::Texture = zombieTex;
        int zombieCount = 0;

};

#endif // ZOMBIE_H
