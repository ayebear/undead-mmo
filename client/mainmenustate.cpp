// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "mainmenustate.h"
#include "playgamestate.h"

MainMenuState MainMenuState::menuState;

MainMenuState::MainMenuState()
{


}
void MainMenuState::init(GameEngine* game)
{

    sf::Vector2f windowSize = static_cast<sf::Vector2f>(game->window.getSize());
    std::string bgFile("data/images/ui/MenuBackground.png");
    std::string fontFile("data/fonts/Ubuntu-B.ttf");
    mainMenu.setUpMenu(bgFile,                             //Background file
                       fontFile,                           //Font file
                       32,                                 //Font size
                       sf::Vector2f(windowSize.x / 1.5, windowSize.y / 2),                 //Rendering window
                       &game->window
                       );



    //Set up menuOption structs
    mainMenu.addMenuButton("Play");
    mainMenu.addMenuButton("Quit");

}
void MainMenuState::cleanup()
{
    mainMenu.clearButtons();
}

void MainMenuState::pause()
{

}
void MainMenuState::resume()
{

}

void MainMenuState::handleEvents(GameEngine* game)
{
     int choice = 0;
     sf::Event event;

    choice = mainMenu.handleEvents(game->window);

     if(choice == 1)
        game->changeState(PlayGameState::instance());
     else if(choice == 2 || choice == -1)
        game->quit();



}
void MainMenuState::update(GameEngine* game)
{
    mainMenu.updateMenu();
}
void MainMenuState::draw(GameEngine* game)
{
    game->window.clear();

    game->window.draw(mainMenu);

    game->window.display();
}


MainMenuState::~MainMenuState()
{
}





