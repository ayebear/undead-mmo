// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "game.h"

int main()
{
    // For now lets just go straight to the game
    Game zombieGame;
    zombieGame.Start();
    // In the future we will instead run the menu, which in turn will start the game
    return 0;
}
