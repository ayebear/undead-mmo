// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "state.h"

const StateAction& State::start(const StateArgs& args)
{
    action.reset(); // Must reset the action in case the state still existed in memory after this returned
    processArgs(args);
    onStart();
    handleEvents();
    while (action.shouldContinue())
    {
        objects.music.update(); //TODO use a callback later
        update();
        draw();
        handleEvents(); // This is last so it doesn't have to update and draw an extra frame if the state is supposed to change
    }
    return action;
}
