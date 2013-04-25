// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entityliving.h"
//#include <cmath>

EntityLiving::EntityLiving()
{
    angle = 0;
    speed = defaultSpeed;
    sprite.setOrigin(32, 32);
    sprite.setPosition(300, 300);
}

void EntityLiving::Move(float deltaTime)
{
    Move(deltaTime, angle);
}

void EntityLiving::Move(float deltaTime, float deg)
{
    angle = deg;
    float rad = (deg * PI) / 180.0;
    sf::Vector2f amount;
    amount.x = deltaTime * speed * cos(rad);
    amount.y = deltaTime * speed * sin(rad);
    sprite.move(amount);
    sprite.setRotation(deg + 90);
    pos = sprite.getPosition();
}
