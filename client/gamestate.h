#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gameengine.h"

/*
*   GameState class. Different game states inherit from this class,
*
*   GameEngine is the gamestate manager
*
*
*
*/
class GameState
{
    public:
      GameState();
      ~GameState();

      virtual void init(GameEngine*) = 0;
      virtual void cleanup() = 0;

      virtual void pause() = 0;
      virtual void resume() = 0;

      virtual void handleEvents(GameEngine*) = 0;
      virtual void update(GameEngine*) = 0;
      virtual void draw(GameEngine*) = 0;

      void changeState(GameEngine*, GameState*);

    protected:
    private:
};

#endif // GAMESTATE_H
