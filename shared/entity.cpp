#include "entity.h"

Entity::Entity()
{
    ID = 0;
    alive = false;
}

void Entity::SetImage(sf::Image& img)
{
    sprite.SetImage(img);
}

void Entity::SetPos(sf::Vector2f& position)
{
    pos = position;
    sprite.SetPosition(pos);
}

void Entity::SetPos(float x, float y)
{
    pos.x = x;
    pos.y = y;
    sprite.SetPosition(pos);
}

int Entity::GetOpenId()
{
    return 0;
}

/*
virtual void draw(sf::RenderTarget& target, RenderStates states) const
{
    //This is required to make the render engine work.
}
*/

