// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "loginstate.h"
#include <iostream>
#include <sstream>
#include "paths.h"

LoginState::LoginState(GameObjects& gameObjects):
    CommonState(gameObjects),
    servers(Paths::masterServersConfig)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.windowSize.x;
    windowSize.y = objects.windowSize.y;

    loginMenu.setUpMenu(Paths::menuBgImage,                                  //Background file
                        sf::Color( 25, 25, 25, 200),
                       16,                                                  //Font size
                       sf::Vector2f(windowSize.x / 4, windowSize.y / 1.3),    //Button position
                       &objects.window,                                     //Rendering window
                       &objects.fontBold
                       );


    textItemList.setupList(objects.window, sf::FloatRect(0, 0, 1, .5), objects.fontBold, 16, true, true);
    textItemList.addTextItem("Server list not loaded.", sf::Color::Blue);
    textItemList.scrollToBottom();


    //Just using these because the width in pixels changes based on window size
    sf::Text usernameWidth("Username:__", objects.fontBold, fontSize);
    float userWidth = usernameWidth.getGlobalBounds().width;

    sf::Text passwordWidth("Password:__", objects.fontBold, fontSize);
    float passWidth = passwordWidth.getGlobalBounds().width;

    sf::Text dirConnectWidth("Direct Connect:__", objects.fontBold, fontSize);
    float directConnWidth = dirConnectWidth.getGlobalBounds().width;


    //Set up menuOption structs
    loginMenu.addMenuButton("Refresh Servers");
    loginMenu.addMenuButton("Login");
    loginMenu.addMenuButton("Create Account");
    loginMenu.addMenuButton("Return to Main Menu");

    loginMenu.createLabel("Username: ", sf::Vector2f(windowSize.x / 4 - userWidth, windowSize.y / 1.5));
    loginMenu.createLabel("Password: ", sf::Vector2f(windowSize.x / 4 - passWidth, windowSize.y / 1.4));
    loginMenu.createLabel("Direct Connect: ", sf::Vector2f(windowSize.x / 1.5 - directConnWidth, windowSize.y / 1.4));

    usernameBox.setUp(fontSize, objects.fontBold, windowSize.x / 4, windowSize.y / 1.5, windowSize.x / 8, false);
    passwordBox.setUp(fontSize, objects.fontBold, windowSize.x / 4, windowSize.y / 1.4, windowSize.x / 8, true);
    directConnectBox.setUp(fontSize, objects.fontBold, windowSize.x / 1.5, windowSize.y / 1.4, windowSize.x / 8, false);

    objects.config.useSection("Credentials");
    string username = objects.config("username").toString();
    string password = objects.config("password").toString();
    string ip = objects.config("server").toString();
    objects.config.useSection();
    usernameBox.setString(username);
    passwordBox.setString(password);
    directConnectBox.setString(ip);
}

LoginState::~LoginState()
{
    loginMenu.clearButtons();
}

void LoginState::onStart()
{
    objects.music.start("menu");
    displayServers();
}

void LoginState::handleEvents()
{
    sf::Event event;
    while(objects.window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                stateEvent.exitGame();
                break;

            case sf::Event::MouseMoved:
                loginMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseWheelMoved:
                textItemList.handleScrolling(event, objects.window);
                break;

            case sf::Event::MouseButtonPressed:

                usernameBox.handleMouseClicked(event, objects.window);
                passwordBox.handleMouseClicked(event, objects.window);
                directConnectBox.handleMouseClicked(event, objects.window);
                loginMenu.handleMousePressed(event);

                if(textItemList.listContainsMouse(objects.window))
                    directConnectBox.setString(textItemList.handleMouseClicked(event, objects.window));
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(loginMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        stateEvent.popState();
                        break;

                    case sf::Keyboard::Return:
                        processChoice(loginMenu.handleKeyPressed(event));
                        break;

                    case sf::Keyboard::Key::Tab:
                        if (usernameBox.isActive())
                        {
                            usernameBox.setInput(false);
                            passwordBox.setInput(true);
                        }
                        else if( passwordBox.isActive())
                        {
                            passwordBox.setInput(false);
                            usernameBox.setInput(true);
                        }
                        break;

                    default:
                        break;
                }
                usernameBox.processInput(event.key.code);
                passwordBox.processInput(event.key.code);
                directConnectBox.processInput(event.key.code);
                loginMenu.handleKeyPressed(event);
                break;

            case sf::Event::TextEntered:
                if(usernameBox.isActive())
                    usernameBox.processTextEntered(event.text.unicode);
                else if(passwordBox.isActive())
                    passwordBox.processTextEntered(event.text.unicode);
                else if(directConnectBox.isActive())
                    directConnectBox.processTextEntered(event.text.unicode);
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
    directConnectBox.updateCursor();
}

void LoginState::processChoice(int choice)
{
    if (choice == 1)
        refreshServers();
    else if (choice == 2)
    {
        string server = directConnectBox.getString();
        sf::IpAddress serverAddr(server);
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Logging into " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.logIn(serverAddr, username, password);
        if (status == Packet::LogInCode::Successful)
            stateEvent.pushState("Game");
        else
        {
            StateArgs args;
            args.push_back(Packet::LogInMessages[status - 1]);
            stateEvent.pushState("Message", args);
        }
    }
    else if (choice == 3)
    {
        string server = directConnectBox.getString();
        sf::IpAddress serverAddr(server);
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Creating account on " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.createAccount(serverAddr, username, password);
        StateArgs args;
        args.push_back(Packet::CreateAccountMessages[status - 1]);
        stateEvent.pushState("Message", args);
    }
    else if (choice == 4)
        stateEvent.popState();
}

void LoginState::update()
{
    objects.music.update();
    loginMenu.updateMenu();
}

void LoginState::draw()
{
    objects.window.clear();
    objects.window.draw(loginMenu);
    objects.window.draw(textItemList);

    objects.window.draw(usernameBox);
    objects.window.draw(passwordBox);
    objects.window.draw(directConnectBox);

    objects.window.display();
}

void LoginState::refreshServers()
{
    textItemList.clearList();
    textItemList.addTextItem("Refreshing servers...", sf::Color::Green);
    textItemList.scrollToBottom();
    draw();

    if (servers.refresh())
        displayServers();
    else
    {
        textItemList.clearList();
        textItemList.addTextItem("Error refreshing servers.", sf::Color::Red);
        textItemList.scrollToBottom();
    }
}

void LoginState::displayServers()
{
    const sf::Color textColor(190, 190, 190, 255);
    textItemList.clearList();
    cfg::File& servercfg = servers.getServerList();
    for (auto& section : servercfg)
    {
        // If there is a port listed, append the colon and the port number
        string port = (servercfg("port", section.first).toString() == "" ? "" : (":" + servercfg("port", section.first).toString()));
        string address = servercfg("address", section.first).toString() + port;
        string displayName = section.first + " (" + address + ")";
        textItemList.addItemWithHiddenText(displayName, address, textColor);
    }

}
