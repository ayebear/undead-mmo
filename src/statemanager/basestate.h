// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef BASESTATE_H
#define BASESTATE_H

#include "stateevent.h"

/*
This is a generic base class for all types of game states.
The state manager uses this polymorphically to switch between states.
*/
class BaseState
{
    public:
        BaseState() {}
        virtual ~BaseState() {}

        // These are totally optional, they don't even need to be implemented by sub classes
        virtual void processArgs(const StateArgs& args) {} // Arguments passed in from the state that asked for the push
        virtual void onPush() {} // Gets called only when the state has been pushed onto the stack
        virtual void onPop() {} // Gets called only when the state has been popped off of the stack
        virtual void onStart() {} // Gets called when the state is started

        const StateEvent& start(const StateArgs&); // Calls processArgs, runs the main loop, then returns the action object

        // These are the main functions of the state, which are called every loop
        virtual void handleEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        StateEvent stateEvent;
};

#endif
