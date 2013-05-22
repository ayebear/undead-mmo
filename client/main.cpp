// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gameengine.h"
#include "mainmenustate.h"
#include <iostream>
int main()
{
    // For now lets just go straight to the game
  GameEngine game;

  // initialize the engine

    game.init(1366, 768, false, "Project: Brains v0.0.2.2 Dev");


  // load the intro
  game.changeState( MainMenuState::instance());

  // main loop
  while ( game.running() )
  {
    game.handleEvents();
    game.update();
    game.draw();
  }

  // cleanup the engine
  game.cleanup();
  return 0;

}
