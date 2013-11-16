// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"

int main()
{
    StateManager game("Undead MMO v0.0.8.2 Dev"); // Create the game
    game.startLoop(StateType::Menu); // Start the game (at the menu)
}

/*
Exit codes:
1xx: Graphics
2xx: Audio
3xx: Fonts
9xx: Other error
*/
