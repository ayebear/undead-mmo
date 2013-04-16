// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

uint Entity::IDCounter = 0;

Entity::Entity()
{
    ID = GetOpenId();
    isReady = false;
}

void Entity::SetTexture(sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Entity::SetPos(sf::Vector2f& position)
{
    pos = position;
    sprite.setPosition(pos);
}

void Entity::SetPos(float x, float y)
{
    pos.x = x;
    pos.y = y;
    sprite.setPosition(pos);
}

sf::Vector2f Entity::GetPos()
{
    return pos;
}

uint Entity::GetOpenId()
{
    // In the future make sure this won't conflict
    return IDCounter++;
    // Need a way to check if an ID isn't used
}

void Entity::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite, states);
}
