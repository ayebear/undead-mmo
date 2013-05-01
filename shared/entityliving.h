// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

#include "entity.h"

const double PI = 3.14159265358979;

class EntityLiving: public Entity
{
    public:
        EntityLiving();
        void Move(float);
        void SetAngle(float);
    protected:
        float angle; // in degrees
        int currentHealth;
        int baseHealth; // max health
        int baseSpeed;
        static const int defaultSpeed = 200;
        float speed;
        bool moving;
};

#endif // ENTITYLIVING_H
