// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"


Map* Entity::mapPtr;

Entity::Entity()
{
    ID = 0;
    ready = false;
}

Entity::~Entity()
{
}

void Entity::setMapPtr(Map& tileMap)
{
    mapPtr = &tileMap;
}
const EID Entity::getID() const
{
    return ID;
}

void Entity::setID(EID newID)
{
    ID = newID;
}

bool Entity::collides(Entity*)
{
    return false;
}

void Entity::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

bool Entity::isMoving()
{
    return false;
}

void Entity::setReady(bool state)
{
    ready = state;
}

const bool Entity::getReady() const
{
    return ready;
}

void Entity::setPos(const sf::Vector2f& position)
{
    pos = position;
    sprite.setPosition(pos);
}

const sf::Vector2f Entity::getPos() const
{
    return pos;
}

sf::Packet& operator<<(sf::Packet& packet, Entity& ent)
{
    return packet << ent.getPacket();
}

sf::Packet& operator>>(sf::Packet& packet, Entity& ent)
{
    return ent.setData(packet);
}
