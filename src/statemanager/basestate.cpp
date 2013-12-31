// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "basestate.h"

// This fuction is used as the main loop in all game states.
const StateEvent& BaseState::start(const StateArgs& args)
{
    stateEvent.reset(); // Must reset the event in case the state still existed in memory after this returned
    processArgs(args);
    onStart();
    handleEvents();
    while (stateEvent.shouldContinue())
    {
        update();
        draw();
        handleEvents(); // This is last so it doesn't have to update and draw an extra frame if the state is supposed to change
    }
    return stateEvent;
}
