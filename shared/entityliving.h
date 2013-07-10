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
        void move(float);
        void setAngle(float);
        void setSpeed(float);
        void setMoving(bool);

    protected:
        void handleCollision();
        void flipAngle();

        static const int defaultSpeed = 10;

        float angle; // in degrees
        float speed; // in pixels per second
        bool moving;

        int currentHealth; // current health
        int baseHealth; // max health
};

#endif
