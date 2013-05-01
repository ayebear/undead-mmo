// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "zombie.h"

Zombie::Zombie()
{
}

/*
We will need to use a pathfinding algorithm like A* to find a random path for the zombie to go.
The zombie will roam that path over time. It will have to calculate new paths depending on state changes.
The tile maps and collision detection will need to be programmed first before this can be started.
*/
void Zombie::Update(float time)
{
    sprite.move(time * speed, 0);
    pos = sprite.getPosition();
}
