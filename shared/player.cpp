#include "player.h"

Player::Player()
{
    speed = defaultSpeed;
}

void Player::MoveUp(float time)
{
    float amount = time * speed;
    sprite.move(0, -amount);
    pos = sprite.getPosition();
}

void Player::MoveDown(float time)
{
    float amount = time * speed;
    sprite.move(0, amount);
    pos = sprite.getPosition();
}

void Player::MoveLeft(float time)
{
    float amount = time * speed;
    sprite.move(-amount, 0);
    pos = sprite.getPosition();
}

void Player::MoveRight(float time)
{
    float amount = time * speed;
    sprite.move(amount, 0);
    pos = sprite.getPosition();
}
