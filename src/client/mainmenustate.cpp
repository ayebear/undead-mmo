// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "mainmenustate.h"
#include <iostream>
#include "paths.h"

MainMenuState::MainMenuState(GameObjects& gameObjects): State(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.windowSize.x;
    windowSize.y = objects.windowSize.y;

    mainMenu.setUpMenu(Paths::menuBgImage,                                  //Background file
                       sf::Color ( 25, 25, 25, 200),
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 1.5, windowSize.y / 2),  //Menu size
                       &objects.window,                                     //Rendering window
                       &objects.fontBold
                       );

    //Set up menuOption structs
    mainMenu.addMenuButton("Play");
    mainMenu.addMenuButton("Quit");
}

MainMenuState::~MainMenuState()
{
    mainMenu.clearButtons();
}

void MainMenuState::onStart()
{
    objects.music.start("Menu");
}

void MainMenuState::handleEvents()
{
    sf::Event event;
    while (objects.window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                stateEvent.exitGame();
                break;

            case sf::Event::MouseMoved:
                mainMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseButtonPressed:
                mainMenu.handleMousePressed(event);
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(mainMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    stateEvent.exitGame();
                else
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
        stateEvent.pushState("Login");
    else if (choice == 2)
        stateEvent.exitGame();
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
