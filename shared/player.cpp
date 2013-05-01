// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "player.h"

Player::Player()
{
    type = Entity::Player;
}

void Player::Update(float time)
{
    if (moving)
        Move(time);
    moving = false; // Quick hack until we have a physics vector thing
}

bool Player::Collides(Entity* ent)
{
    return false;
}

void Player::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}
