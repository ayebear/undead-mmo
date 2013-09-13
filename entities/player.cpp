// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "player.h"

const EType Player::type = Entity::Player;

Player::Player()
{
    speed = 400;
    visualAngle = 0;
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

void Player::getData(sf::Packet& packet)
{
    packet << id << type << pos.x << pos.y << angle << speed << moving << currentHealth << baseHealth << visualAngle;
}

void Player::setData(sf::Packet& packet)
{
    packet >> pos.x >> pos.y >> angle >> speed >> moving >> currentHealth >> baseHealth >> visualAngle;
    sprite.setPosition(pos);
    updateSpriteRotation();
}

float Player::getVisualAngle() const
{
    return visualAngle;
}

void Player::setVisualAngle(float ang)
{
    visualAngle = ang;
    updateSpriteRotation();
    changed = true;
}

void Player::updateSpriteRotation()
{
    sprite.setRotation(visualAngle + 90);
}

EType Player::getType() const
{
    return type;
}
