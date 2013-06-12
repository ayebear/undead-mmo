// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entityliving.h"
//#include <cmath>

EntityLiving::EntityLiving()
{
    angle = 0;
    speed = defaultSpeed;
    sprite.setOrigin(32, 32);
    sprite.setPosition(400, 300);
    moving = false;
}

void EntityLiving::Move(float deltaTime)
{
    float rad = (angle * PI) / 180.0;
    sf::Vector2f amount;
    amount.x = deltaTime * speed * cos(rad);
    amount.y = deltaTime * speed * sin(rad);
    sprite.move(amount);
    sprite.setRotation(angle + 90);
    pos = sprite.getPosition();
    // TODO: Improve this later
    if (pos.x < 0)
    {
        pos.x = 0;
        Move(-deltaTime);
    }
    if (pos.y < 0)
    {
        pos.y = 0;
        Move(-deltaTime);
    }

    if (pos.x >= mapPtr->getMapWidth())
    {
        pos.x = mapPtr->getMapWidth();
        Move(-deltaTime);
    }
    if (pos.y >= mapPtr->getMapHeight())
    {
        pos.y = mapPtr->getMapHeight();
        Move(-deltaTime);
    }
}

void EntityLiving::SetAngle(float deg)
{
    angle = deg;
    moving = true;
}

void EntityLiving::SetSpeed(float theSpeed)
{
    speed = theSpeed;
}

void EntityLiving::SetMoving(bool isMoving)
{
    moving = isMoving;
}
