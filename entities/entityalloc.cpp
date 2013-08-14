// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "entityalloc.h"
#include "player.h"
#include "zombie.h"

// This function allocates a new specific entity object from a type and returns its pointer
Entity* allocateEntity(int type)
{
    switch (type)
    {
        case Entity::Player:
            return new Player;
        case Entity::Zombie:
            return new Zombie;
        default:
            return nullptr;
    }
}
