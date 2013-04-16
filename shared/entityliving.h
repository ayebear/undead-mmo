// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

#include "entity.h"

class EntityLiving: public Entity
{
    public:
        EntityLiving();
        void Move(float);
    protected:
        float angle; // in degrees
        int currentHealth;
        int baseHealth; // max health
        int baseSpeed;
};

#endif // ENTITYLIVING_H
