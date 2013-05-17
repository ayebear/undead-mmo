#include "gamestate.h"

GameState::GameState()
{
    //ctor
}

GameState::~GameState()
{
    //dtor
}

void GameState::changeState(GameEngine* game, GameState* state) {
    game->changeState(state);
}
