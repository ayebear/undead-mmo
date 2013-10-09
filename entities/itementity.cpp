#include "itementity.h"

ItemEntity::ItemEntity()
{
    type = Entity::Item;
    setTexture(type);
}

void ItemEntity::update(float time)
{

}

bool ItemEntity::collides(Entity* ent)
{
    return false;
}

void ItemEntity::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}

void ItemEntity::getData(sf::Packet&)
{

}

void ItemEntity::setData(sf::Packet&)
{

}
