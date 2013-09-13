// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "zombie.h"

const EType Zombie::type = Entity::Zombie;

Zombie::Zombie()
{
    speed = 50;
}

/*
We will need to use a pathfinding algorithm like A* to find a random path for the zombie to go.
The zombie will roam that path over time. It will have to calculate new paths depending on state changes.
The tile maps and collision detection will need to be programmed first before this can be started.
*/
void Zombie::update(float time)
{
    move(time);
}

bool Zombie::collides(Entity* ent)
{
    return false;
}

void Zombie::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}

void Zombie::getData(sf::Packet& packet)
{
    packet << id << type << pos.x << pos.y << angle << speed << moving << currentHealth << baseHealth;
}

void Zombie::setData(sf::Packet& packet)
{
    packet >> pos.x >> pos.y >> angle >> speed >> moving >> currentHealth >> baseHealth;
    sprite.setPosition(pos);
}

EType Zombie::getType() const
{
    return type;
}
