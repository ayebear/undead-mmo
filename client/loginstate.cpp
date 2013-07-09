// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include <sstream>
#include "loginstate.h"

LoginState::LoginState(GameObjects& gameObjects): State(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.window.getSize().x;
    windowSize.y = objects.window.getSize().y;

    std::string bgFile("data/images/ui/MenuBackground.png");

    loginMenu.setUpMenu(bgFile,                                             //Background file
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 5, windowSize.y / 2),    //Button position
                       objects                                              //Rendering window
                       );


    textItemList.setupList(objects.window, sf::FloatRect(windowSize.x / 1.5, windowSize.y / 6, windowSize.x / 3, windowSize.y / 1.5), gameObjects.fontBold, 16, true);

    std::string test("Kevin Millerajk");
    textItemList.addTextItem(test);
    textItemList.addTextItem("This is a test...");

    for (int x = 1; x <= 100; x++)
    {
        std::stringstream tmp;
        tmp << "Test server " << x;
        textItemList.addTextItem(tmp.str(), sf::Color::Red);
    }

    textItemList.addTextItem("This is to test the wrapping feature to make sure the text is wrapped properly......                                                                                                                                                                                         With multiple spaces and veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long words.");

  //  textItemList.addTextItem(test2);
    //Set up menuOption structs
    loginMenu.addMenuButton("Login");
    loginMenu.addMenuButton("Return to Main Menu");
}

LoginState::~LoginState()
{
    loginMenu.clearButtons();
}

void LoginState::handleEvents()
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
                loginMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseWheelMoved:
                textItemList.handleScrolling(event, objects.window);
                break;

            case sf::Event::MouseButtonPressed:
                textItemList.handleMouseClicked(event, objects.window);
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(loginMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    action.popState();
                else
                    processChoice(loginMenu.handleKeyPressed(event));
                break;

            case sf::Event::Resized:
                loginMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
}

void LoginState::processChoice(int choice)
{
    if (choice == 1)
        action.pushState(StateType::Game);
    else if (choice == 2)
        action.popState();
}

void LoginState::update()
{
    loginMenu.updateMenu();
}

void LoginState::draw()
{
    objects.window.clear();

    objects.window.draw(loginMenu);
    objects.window.draw(textItemList);

    objects.window.display();
}
