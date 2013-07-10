// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

int Entity::mapWidth = 0;
int Entity::mapHeight = 0;

Entity::Entity()
{
    ID = 0;
    ready = false;
}

Entity::~Entity()
{
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

void Entity::moveTo(const sf::Vector2f& posToMove)
{
    setPos(posToMove);
}

void Entity::setMapSize(int width, int height)
{
    mapWidth = width;
    mapHeight = height;
}

sf::Packet& operator<<(sf::Packet& packet, Entity& ent)
{
    return packet << ent.getPacket();
}

sf::Packet& operator>>(sf::Packet& packet, Entity& ent)
{
    return ent.setData(packet);
}
