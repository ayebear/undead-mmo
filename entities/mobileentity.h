// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MOBILEENTITY_H
#define MOBILEENTITY_H

#include "entity.h"

const double PI = 3.14159265358979;

class MobileEntity: public Entity
{
    public:
        MobileEntity();
        void move(float);
        void setAngle(float);
        void setSpeed(float);
        void setMoving(bool);
        virtual void updateSpriteRotation();

    protected:
        void handleCollision();
        void flipAngle();

        static const int defaultSpeed = 10;

        float angle; // in degrees
        float speed; // in pixels per second
        bool moving;

        sf::Int32 currentHealth; // current health
        sf::Int32 baseHealth; // max health
};

#endif
