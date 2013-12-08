// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "mobileentity.h"

MobileEntity::MobileEntity()
{
    angle = 0;
    speed = defaultSpeed;
    sprite.setOrigin(32, 32);
    sprite.setPosition(400, 300);
    moving = false;
}

void MobileEntity::move(float deltaTime)
{
    if (moving)
    {
        float rad = (angle * PI) / 180.0;
        sf::Vector2f amount;
        amount.x = deltaTime * speed * cos(rad);
        amount.y = deltaTime * speed * sin(rad);
        sprite.move(amount);
        updateSpriteRotation();
        pos = sprite.getPosition();
        handleCollision();
    }
}

void MobileEntity::setAngle(float deg)
{
    angle = deg;
    changed = true;
}

void MobileEntity::setSpeed(float theSpeed)
{
    speed = theSpeed;
    changed = true;
}

void MobileEntity::setMoving(bool isMoving)
{
    moving = isMoving;
    changed = true;
}

void MobileEntity::updateSpriteRotation()
{
    sprite.setRotation(angle + 90);
}

void MobileEntity::handleCollision()
{
    // TODO: Improve this later
    if (pos.x < 0)
    {
        pos.x = 0;
        flipAngle();
    }

    if (pos.y < 0)
    {
        pos.y = 0;
        flipAngle();
    }

    if (pos.x >= mapWidth)
    {
        pos.x = mapWidth;
        flipAngle();
    }

    if (pos.y >= mapHeight)
    {
        pos.y = mapHeight;
        flipAngle();
    }

    sprite.setPosition(pos);
}

void MobileEntity::flipAngle()
{
    if (type == Zombie)
    {
        changed = true;
        angle += 90;
        if (angle > 360)
            angle -= 360;
    }
}
