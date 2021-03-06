// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATE_H
#define STATE_H

#include "basestate.h"
#include "gameobjects.h"

/*
This class contains a few specific things that are common across the game states.
This is needed to avoid redundant code and messing up the base class.
*/
class CommonState: public BaseState
{
    public:
        CommonState(GameObjects& gameObjects): objects(gameObjects) {}

    protected:
        GameObjects& objects;
};

#endif
