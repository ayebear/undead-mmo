// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entity.h"

EID Entity::IDCounter = 0;

Entity::Entity()
{
    ID = GetOpenID();
    isReady = false;
}

Entity::~Entity()
{
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

sf::Vector2f Entity::GetPos()
{
    return pos;
}

const EID Entity::GetID()
{
    return ID;
}

EID Entity::GetOpenID()
{
    // In the future make sure this won't conflict
    return IDCounter++;
    // Need a way to check if an ID isn't used
}

void Entity::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite, states);
}

/*
#include "entity.h"

Entity::Entity()
{
    if(ents == NULL)
    {
   	 ents = new EntityList;
    }

    std::array<int,2> temp;
    temp.fill(2);
    setPosition(temp);
    setReady(false);
    ents->insertEntity(*this);
}

Entity::~Entity()
{
    ents->deleteEntity(*this);
}

bool Entity::setPosition(std::array<int,2> vec)//,Map);
{
    position = vec;

    //IF vec is in map return true, else return false.
}

std::array<int,2> Entity::getPosition() const
{
    return position;
}

void Entity::update()
{
    std::printf("Entity {0} is updated.",_ID);
}

void Entity::setReady(bool r)
{
    isReady = r;
}
bool Entity::getReady() const
{
    return isReady;
}
    //virtual bool draw();

bool Entity::operator==(const Entity &other) const
{
    if(this->_ID == other._ID)
   	 return true;
    return false;
}

bool Entity::operator<=(const Entity &other) const
{
    if(this->_ID <= other._ID)
   	 return true;
    return false;
}
bool Entity::operator>=(const Entity &other) const
{
    if(this->_ID >= other._ID)
   	 return true;
    return false;
}
bool Entity::operator!=(const Entity &other) const
{
    if(this->_ID != other._ID)
   	 return true;
    return false;
}
bool Entity::operator<(const Entity &other) const
{
    if(this->_ID < other._ID)
   	 return true;
    return false;
}
bool Entity::operator>(const Entity &other) const
{
    if(this->_ID > other._ID)
   	 return true;
    return false;
}
*/


/* OLD
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
*/
