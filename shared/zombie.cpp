// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "zombie.h"

Zombie::Zombie()
{
    //Zombie Constructor

}

void Zombie::freeRoam(float time) {
   float amount = time * speed;
   //random number generator for directions
   //while (pos is valid)
   //{
   pos = sprite.getPosition();
   sprite.move(0, -amount);
   pos = sprite.getPosition();
   //}


}
