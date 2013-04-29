// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "entityliving.h"

/*
Zombies will need a "state" so we know what they are supposed to be doing.
*/
class Zombie: public EntityLiving
{
    public:
        Zombie();
        void Update(float);
    private:
};

#endif
