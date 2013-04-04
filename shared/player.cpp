#include "player.h"

Player::Player()
{
    speed = defaultSpeed;
}

void Player::MoveUp(float time)
{
    float amount = time * speed;
    sprite.Move(0, -amount);
    pos = sprite.GetPosition();
}

void Player::MoveDown(float time)
{
    float amount = time * speed;
    sprite.Move(0, amount);
    pos = sprite.GetPosition();
}

void Player::MoveLeft(float time)
{
    float amount = time * speed;
    sprite.Move(-amount, 0);
    pos = sprite.GetPosition();
}

void Player::MoveRight(float time)
{
    float amount = time * speed;
    sprite.Move(amount, 0);
    pos = sprite.GetPosition();
}
