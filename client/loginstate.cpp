#include "loginstate.h"
#include "mainmenustate.h"
#include <iostream>
LoginState LoginState::loginState;

LoginState::LoginState()
{
    //ctor
}

LoginState::~LoginState()
{
    //dtor
}

void LoginState::init(GameEngine* game)
{
    sf::Vector2f windowSize;
    windowSize.x = game->window.getSize().x;
    windowSize.y = game->window.getSize().y;

    std::string bgFile("data/images/ui/MenuBackground.png");
    std::string fontFile("data/fonts/Ubuntu-B.ttf");
    loginMenu.setUpMenu(bgFile,                                             //Background file
                       fontFile,                                            //Font file
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 5, windowSize.y / 2),  //Button position
                       &game->window                                        //Rendering window
                       );


    textItemList.setupList(game->window, sf::FloatRect(windowSize.x / 1.5, windowSize.y / 6, windowSize.x / 3, windowSize.y / 1.5), fontFile, 16);

    std::string test("Kevin Millerajksdjkansjkdnkjanskdjnajksdjknasjkdnjkaasdsdasdngfhjklnakjsndjkasnjkasdj knjaksddddddddddddndjkasnjkasdjkn jaksddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddndjka snjkasdjknjaksdddddddddddddddddddddddddddddddddddd ddddddddddddddddddddddddddddddddddddddddddddddddddddddndjkasnjkandjkasnjkasdjknjaksddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddsdjknjaksddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddkasndkjanskjdnauiwndyuabsuyfbasdfhyujbasdhjfbhjuasdfk");
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);
/*

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);


    test = "Kevin Miller";
    textItemList.addTextItem(test);

    test = "Kevin Miller";
    textItemList.addTextItem(test);*/


  //  textItemList.addTextItem(test2);
    //Set up menuOption structs
    loginMenu.addMenuButton("Login");
    loginMenu.addMenuButton("Return to Main Menu");

}
void LoginState::cleanup()
{
    loginMenu.clearButtons();
}
void LoginState::pause()
{

}
void LoginState::resume()
{

}
void LoginState::handleEvents(GameEngine* game)
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
                loginMenu.handleMouseMovement(event);
                break;
            }
            case sf::Event::MouseWheelMoved:
                textItemList.handleScrolling(event, game->window);
            case sf::Event::MouseButtonPressed:
                textItemList.handleMouseClicked(event, game->window);
            case sf::Event::MouseButtonReleased:
            {
                choice = loginMenu.handleMouseReleased(event, game->window);
                if(choice == 1)
                    std::cout << "Logging in...\n";
                else if(choice == 2)
                    game->changeState(MainMenuState::instance());
                break;
            }

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
            {
                choice = loginMenu.handleKeyPressed(event, game->window);
                if(choice == 1)
                    std::cout << "Logging in...\n";
                else if(choice == 2)
                    game->changeState(MainMenuState::instance());
                break;
            }

            case sf::Event::Resized:
            {
                loginMenu.handleResize(event, game->window);
                break;
            }
            default:
                break;
            }
        }
}
void LoginState::update(GameEngine* game)
{
    loginMenu.updateMenu();
}
void LoginState::draw(GameEngine* game)
{
    game->window.clear();

    game->window.draw(loginMenu);
    game->window.draw(textItemList);

    game->window.display();
}
