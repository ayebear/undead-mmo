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
}

/*
virtual void draw(sf::RenderTarget& target, RenderStates states) const
{
    //This is required to make the render engine work.
}
*/

