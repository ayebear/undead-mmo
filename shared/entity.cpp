// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

Entity::Entity()
{
    ID = 0;
    ready = false;
}

Entity::~Entity()
{
}

const EID Entity::GetID() const
{
    return ID;
}

void Entity::SetID(EID newID)
{
    ID = newID;
}

bool Entity::Collides(Entity*)
{
    return false;
}

void Entity::SetTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

bool Entity::IsMoving()
{
    return false;
}

void Entity::SetReady(bool state)
{
    ready = state;
}

const bool Entity::GetReady() const
{
    return ready;
}

void Entity::SetPos(const sf::Vector2f& position)
{
    pos = position;
    sprite.setPosition(pos);
}

const sf::Vector2f Entity::GetPos() const
{
    return pos;
}

sf::Packet& operator<<(sf::Packet& packet, Entity& ent)
{
    return packet << ent.GetPacket();
}

sf::Packet& operator>>(sf::Packet& packet, Entity& ent)
{
    return ent.SetData(packet);
}
