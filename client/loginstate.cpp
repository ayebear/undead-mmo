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


    textItemList.setupList(objects.window, sf::FloatRect(.5, .1, .35, .75), gameObjects.fontBold, 16, true, true);


    textItemList.addTextItem(test);

    textItemList.addTextItem("This is a test...");

    for (int x = 1; x <= 150; x++)
    {
        std::stringstream tmp;
        tmp << "Test server " << x;
        textItemList.addTextItem(tmp.str(), sf::Color(190, 190, 190, 255));
    }

  //  textItemList.addTextItem("This is to test the wrapping feature to make sure the text is wrapped properly......                                                                                                                                                                                         With multiple spaces and veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long words.");

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
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        action.popState();
                        break;

                    case sf::Keyboard::Return:
                        processChoice(loginMenu.handleKeyPressed(event));
                        break;

                    case sf::Keyboard::Key::F1:
                        takeScreenshot();
                        break;
                }
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
    {
        // TODO: Change this later so the options (if any) are loaded into the GUI elements or something like that
        string server = objects.config.getOption("server").asString();
        sf::IpAddress serverAddr(server);
        string username = objects.config.getOption("username").asString();
        string password = objects.config.getOption("password").asString();
        cout << "Logging into " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.logIn(serverAddr, username, password);
        if (status == Packet::Login::Successful)
            action.pushState(StateType::Game);
        else
        {
            // TODO: Show a better error from the actual status code
            StateArgs args;
            args.push_back("Error logging into the server!");
            action.pushState(StateType::Error, args);
        }
    }
    else if (choice == 2)
        action.popState();
}

void LoginState::update()
{
    loginMenu.updateMenu();
}

void LoginState::takeScreenshot()
{
    //Get the current system time.
    time_t currTime = time(0);
    string fileName = "data/screenshots/";
    stringstream ss;
    ss << currTime;

    //Add the time.png to the end of the file name and save it.
    fileName += ss.str() + ".png";
    sf::Image scrShot = objects.window.capture();
    scrShot.saveToFile(fileName);
}

void LoginState::draw()
{
    objects.window.clear();

    objects.window.draw(loginMenu);
    objects.window.draw(textItemList);

    objects.window.display();
}
