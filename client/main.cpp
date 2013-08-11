// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"

int main()
{
    const string title = "Undead MMO v0.0.7.1 Dev";

    StateManager game(title); // Create the game

    StateAction action;
    action.pushState(StateType::Menu); // This will make it go to the menu first

    game.startLoop(action); // Start the game (at the menu)
}

/*
Exit codes:
1xx: Graphics
2xx: Audio
3xx: Fonts
9xx: Other error
*/
