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
                       16,                                                  //Font size
                       sf::Vector2f(windowSize.x / 2 - (windowSize.x / 16), windowSize.y / 1.2),    //Button position
                       objects                                              //Rendering window
                       );


    textItemList.setupList(objects.window, sf::FloatRect(0, 0, .5, .5), gameObjects.fontBold, 16, true, true);

    textItemList.addTextItem("This is a test...");

    for (int x = 1; x <= 150; x++)
    {
        std::stringstream tmp;
        tmp << "Test server " << x << "\t\t\tDescription: This is a game server where we play zombies!!!!!\t\t\tPlayers: 30/32";
        textItemList.addTextItem(tmp.str(), sf::Color(190, 190, 190, 255));
    }

  //  textItemList.addTextItem("This is to test the wrapping feature to make sure the text is wrapped properly......                                                                                                                                                                                         With multiple spaces and veryyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy long words.");

  //  textItemList.addTextItem(test2);
    //Set up menuOption structs
    loginMenu.addMenuButton("Login");
    loginMenu.addMenuButton("Create Account");
    loginMenu.addMenuButton("Return to Main Menu");
    usernameBox.setUp(16, objects.fontBold, windowSize.x / 2 - (windowSize.x / 16), windowSize.y / 1.5, windowSize.x / 8, false);
    passwordBox.setUp(16, objects.fontBold, windowSize.x / 2 - (windowSize.x / 16), windowSize.y / 1.4, windowSize.x / 8, true);
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
                usernameBox.handleMouseClicked(event, objects.window);
                passwordBox.handleMouseClicked(event, objects.window);
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

                    default:
                        break;
                }
                usernameBox.processInput(event.key.code);
                passwordBox.processInput(event.key.code);
                loginMenu.handleKeyPressed(event);
                break;

            case sf::Event::TextEntered:
                if(usernameBox.isActive())
                    usernameBox.processTextEntered(event.text.unicode);
                else if(passwordBox.isActive())
                    passwordBox.processTextEntered(event.text.unicode);
                break;

            case sf::Event::Resized:
                loginMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
    usernameBox.updateCursor();
    passwordBox.updateCursor();
}

void LoginState::processChoice(int choice)
{
    if (choice == 1)
    {
        // TODO: Change this later so the options (if any) are loaded into the GUI elements or something like that
        string server = objects.config.getOption("server").asString();
        sf::IpAddress serverAddr(server);
        //string username = objects.config.getOption("username").asString();
        //string password = objects.config.getOption("password").asString();
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Logging into " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.logIn(serverAddr, username, password);
        if (status == Packet::LogInCode::Successful)
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
    {
        string server = objects.config.getOption("server").asString();
        sf::IpAddress serverAddr(server);
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Creating account on " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.createAccount(serverAddr, username, password);
        if (status != Packet::CreateAccountCode::Successful)
            //action.pushState(StateType::Game);
    //    else
        {
            // TODO: Show a better error from the actual status code
            StateArgs args;
            args.push_back("Error creating account on the server!");
            action.pushState(StateType::Error, args);
        }
    }
    else if (choice == 3)
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

    objects.window.draw(usernameBox);
    objects.window.draw(passwordBox);

    objects.window.display();
}
