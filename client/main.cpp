// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "statemanager.h"

int main()
{
    StateManager game("Project: Brains v0.0.3.0 Dev", 1366, 768);
    StateAction action;
    action.pushState(StateType::Menu);
    game.startLoop(action);
}
