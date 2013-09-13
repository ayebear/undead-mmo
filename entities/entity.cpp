// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

int Entity::mapWidth = 0;
int Entity::mapHeight = 0;
const EType Entity::type = Entity::Invalid;

Entity::Entity()
{
    id = 0;
    ready = false;
    changed = true;
}

Entity::~Entity()
{
}

const EID Entity::getID() const
{
    return id;
}

void Entity::setID(EID newId)
{
    id = newId;
}

EType Entity::getType() const
{
    return type;
}

bool Entity::collides(Entity*)
{
    return false;
}

void Entity::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture);
}

bool Entity::isMoving() const
{
    return false;
}

void Entity::setReady(bool state)
{
    ready = state;
}

bool Entity::getReady() const
{
    return ready;
}

void Entity::setChanged(bool state)
{
    changed = state;
}

bool Entity::hasChanged() const
{
    return changed;
}

float Entity::getVisualAngle() const
{
    return 0;
}

void Entity::setPos(const sf::Vector2f& position)
{
    pos = position;
    sprite.setPosition(pos);
    changed = true;
}

const sf::Vector2f& Entity::getPos() const
{
    return pos;
}

void Entity::moveTo(const sf::Vector2f& posToMove)
{
    setPos(posToMove);
}

const ItemCode& Entity::getItem()
{
    return ItemCode::noItem;
}

void Entity::setMapSize(int width, int height)
{
    mapWidth = width;
    mapHeight = height;
}
