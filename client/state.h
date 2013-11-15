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

        // These are totally optional, they don't even need to be implemented by sub classes
        virtual void processArgs(const StateArgs& args) {} // Arguments passed in from the state that asked for the push
        virtual void onPush() {} // Gets called only when the state has been pushed onto the stack
        virtual void onPop() {} // Gets called only when the state has been popped off of the stack
        virtual void onStart() {} // Gets called when the state is started

        const StateAction& start(const StateArgs&); // Calls processArgs, runs the main loop, then returns the action object

        // These are the main functions of the state, which are called every loop
        virtual void handleEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        StateAction action;
        GameObjects& objects;
};

#endif
