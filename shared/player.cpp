// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "player.h"

Player::Player()
{
    type = Entity::Player;
    speed = 800;
}

void Player::update(float time)
{
    move(time);
}

bool Player::collides(Entity* ent)
{
    return false;
}

void Player::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}

sf::Packet Player::getPacket()
{
    sf::Packet packet;
    packet << ID << type << pos.x << pos.y;
    return packet;
}

sf::Packet& Player::setData(sf::Packet& packet)
{
    return packet >> ID >> type >> pos.x >> pos.y;
}
