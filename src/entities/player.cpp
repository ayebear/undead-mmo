// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "player.h"

PlayerEntity::PlayerEntity()
{
    type = Entity::Player;
    speed = 400;
    visualAngle = 0;
    setTexture(type);
}

void PlayerEntity::update(float time)
{
    move(time);
}

bool PlayerEntity::collides(Entity* ent)
{
    return false;
}

void PlayerEntity::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}

void PlayerEntity::getData(sf::Packet& packet)
{
    packet << id << type << pos.x << pos.y << angle << speed << moving << currentHealth << baseHealth << visualAngle;
}

void PlayerEntity::setData(sf::Packet& packet)
{
    packet >> pos.x >> pos.y >> angle >> speed >> moving >> currentHealth >> baseHealth >> visualAngle;
    sprite.setPosition(pos);
    updateSpriteRotation();
}

float PlayerEntity::getVisualAngle() const
{
    return visualAngle;
}

void PlayerEntity::setVisualAngle(float ang)
{
    visualAngle = ang;
    updateSpriteRotation();
    changed = true;
}

void PlayerEntity::updateSpriteRotation()
{
    sprite.setRotation(visualAngle + 90);
}
