// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "errorstate.h"
#include <iostream>
#include <sstream>
#include "paths.h"

ErrorState::ErrorState(GameObjects& gameObjects): State(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.windowSize.x;
    windowSize.y = objects.windowSize.y;

    errorMenu.setUpMenu(Paths::menuBgImage,                              //Background file
                        sf::Color (25, 25, 25, 200),
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 5, windowSize.y / 2),    //Button position
                       objects                                     //Rendering window
                       );

    errorMenu.addMenuButton("Go Back (this may fail)");
    errorMenu.addMenuButton("Report Bug");
    errorMenu.addMenuButton("Restart Game");
    errorMenu.addMenuButton("Exit Game");

    errorFont.loadFromFile(Paths::boldFont);

    headingText.setFont(errorFont);
    headingText.setString("An error has occurred, the details are below:");
    headingText.setCharacterSize(40);
    headingText.setColor(sf::Color::Red);
    headingText.setPosition(40, 40);

    errorText.setFont(errorFont);
    errorText.setCharacterSize(40);
    errorText.setColor(sf::Color::Green);
    errorText.setPosition(40, 200);
}

ErrorState::~ErrorState()
{
    errorMenu.clearButtons();
}

void ErrorState::processArgs(const StateArgs& args)
{
    if (args.empty())
        errorText.setString("No details were returned.");
    else
        errorText.setString(args.front());
}

void ErrorState::handleEvents()
{
    sf::Event event;
    while(objects.window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                action.exitGame();
                break;

            case sf::Event::MouseMoved:
                errorMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(errorMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    action.popState();
                else
                    processChoice(errorMenu.handleKeyPressed(event));
                break;

            case sf::Event::Resized:
                errorMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
}

void ErrorState::processChoice(int choice)
{
    if (choice == 1)
        action.popState();
    else if (choice > 1)
        action.exitGame();
}

void ErrorState::update()
{
    errorMenu.updateMenu();
}

void ErrorState::draw()
{
    objects.window.clear();

    objects.window.draw(errorMenu);
    objects.window.draw(headingText);
    objects.window.draw(errorText);

    objects.window.display();
}
