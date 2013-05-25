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

    sf::Vector2f windowSize;
    windowSize.x = game->window.getSize().x;
    windowSize.y = game->window.getSize().y;

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
        while(game->window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                game->quit();
                break;

            case sf::Event::MouseMoved:
            {
                mainMenu.handleMouseMovement(event);
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                choice = mainMenu.handleMouseReleased(event, game->window);
                if(choice == 1)
                    game->changeState(PlayGameState::instance());
                else if(choice == 2)
                    game->quit();
                break;
            }

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
            {
                choice = mainMenu.handleKeyPressed(event, game->window);
                if(choice == 1)
                    game->changeState(PlayGameState::instance());
                else if(choice == 2)
                    game->quit();
                break;
            }

            case sf::Event::Resized:
            {
                mainMenu.handleResize(event, game->window);
                break;
            }
            default:
                break;
            }
        }


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





