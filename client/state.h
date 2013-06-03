// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATE_H
#define STATE_H

#include "stateaction.h"
#include "gameobjects.h"

/*
This is the base class for all types of game states.
*/
class State
{
    public:
        State(GameObjects& gameObjects): objects(gameObjects) {}
        virtual ~State() {}

        const StateAction& start(const StateArgs&); // Calls processArgs, runs the main loop, then returns the action object

        virtual void processArgs(const StateArgs& args) {} // Totally optional, doesn't even need to be implemented by sub class

        virtual void handleEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        StateAction action;
        GameObjects& objects;
};

#endif
