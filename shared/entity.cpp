// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

Entity::Entity()
{
    ID = 0;
    isReady = false;
}

Entity::~Entity()
{
}

const EID Entity::GetID()
{
    return ID;
}

void Entity::SetID(EID newID)
{
    ID = newID;
}

void Entity::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite, states);
}




void Entity::SetTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

void Entity::SetPos(const sf::Vector2f& position)
{
    pos = position;
    sprite.setPosition(pos);
}

sf::Vector2f Entity::GetPos()
{
    return pos;
}

