// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"

int main()
{
    const string title = "Project: Brains v0.0.4.5 Dev";

    StateManager game(title); // Create the game

    StateAction action;
    action.pushState(StateType::Menu); // This will make it go to the menu first

    game.startLoop(action); // Start the game
}
