// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "mainmenustate.h"

MainMenuState::MainMenuState(GameObjects& gameObjects): State(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.window.getSize().x;
    windowSize.y = objects.window.getSize().y;

    mainMenu.setUpMenu("data/images/ui/MenuBackground.png",                 //Background file
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 1.5, windowSize.y / 2),  //Menu size
                       objects                                              //Rendering window
                       );

    //Set up menuOption structs
    mainMenu.addMenuButton("Play");
    mainMenu.addMenuButton("Quit");
}

MainMenuState::~MainMenuState()
{
    mainMenu.clearButtons();
}

void MainMenuState::handleEvents()
{
    sf::Event event;
    while (objects.window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                action.exitGame();
                break;

            case sf::Event::MouseMoved:
                mainMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(mainMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                processChoice(mainMenu.handleKeyPressed(event));
                break;

            case sf::Event::Resized:
                mainMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
}

void MainMenuState::processChoice(int choice)
{
    if (choice == 1)
    {
        std::cout << "MainMenuState created a push action to LoginState.\n";
        action.pushState(StateType::Login);
    }
    else if (choice == 2)
        action.exitGame();
}

void MainMenuState::update()
{
    mainMenu.updateMenu();
}

void MainMenuState::draw()
{
    objects.window.clear();

    objects.window.draw(mainMenu);

    objects.window.display();
}
