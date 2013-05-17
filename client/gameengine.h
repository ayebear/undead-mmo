#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

/*
*   GameEngine class manages every gamestate. The gamestates themselves decide when to change to
*   a different gamestate.
*
*   This class contains the window and main video mode as well.
*/

class GameState;

class GameEngine
{
    public:
      GameEngine();
      ~GameEngine();

      //Window width, height, fullscreen, and window title
      void init(int windowWidth = 800, int windowHeight = 600, bool fullscreen = false, std::string title = "Zombie Game");
      void cleanup();

      void changeState(GameState*);
      void pushState(GameState*);
      void popState();

      void handleEvents();
      void update();
      void draw();

      bool running();
      void quit();


      sf::VideoMode vidMode;
      sf::RenderWindow window;

    private:
      // the stack of states
      std::vector<GameState*> states;


      bool isRunning;
};

#endif // GAMEENGINE_H
