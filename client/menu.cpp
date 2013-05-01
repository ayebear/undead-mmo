// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "menu.h"
#include <iostream>
Menu::Menu(sf::RenderWindow& screen, sf::VideoMode videoMode)
{


    if(!background.loadFromFile("data/images/ui/MenuBackground.png"))
        exit(Errors::Graphics);

    if(!font.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(Errors::Font);

    background.setSmooth(true);
    backgroundImage.setTexture(background);

//scale take a factor amount so  newSize/oldSize.
//The background image I made is 1920 x 1080, so resize it for windowed users.  Move the origin because scaling the image moves it.
    backgroundImage.setScale(static_cast<float>(videoMode.width) / 1920, static_cast<float>(videoMode.height) / 1080);
    backgroundImage.setOrigin(0, 0);


    view.reset(sf::FloatRect(0, 0, videoMode.width, videoMode.height));


    topOptionPos.x = videoMode.width / 1.5;                     //Smaller numbers puts text further the the right. Larger goes left.
    topOptionPos.y = videoMode.height / 2;


    fontSize = 32;

    unselectedColor = sf::Color::White;
    selectedColor = sf::Color::Yellow;
    selection = 0;

    //Set up menuOption structs

    play.option.setString("Play");
    play.option.setCharacterSize(fontSize);
    play.option.setColor(unselectedColor);
    play.option.setFont(font);
    play.option.setPosition(topOptionPos.x, topOptionPos.y);

    tmpText = play.option.getString();

    //Rectangle to make mouse collision easier
    play.rect.left = topOptionPos.x;
    play.rect.top = topOptionPos.y;
    play.rect.width = (tmpText.length() * fontSize / 1.8);            //Adjust rectangles width based on the text
    play.rect.height = fontSize;


    options.option.setString("Options");
    options.option.setCharacterSize(fontSize);
    options.option.setColor(unselectedColor);
    options.option.setFont(font);
    options.option.setPosition(topOptionPos.x, topOptionPos.y + fontSize + 75);   //Extra 75 pixels of space

    tmpText = options.option.getString();

    options.rect.left = topOptionPos.x;
    options.rect.top = topOptionPos.y + fontSize + 75;
    options.rect.width = (tmpText.length() * fontSize / 1.8);         //Adjust rectangles width based on the text
    options.rect.height = fontSize;


    quit.option.setString("Quit");
    quit.option.setCharacterSize(fontSize);
    quit.option.setColor(unselectedColor);
    quit.option.setFont(font);
    quit.option.setPosition(topOptionPos.x, topOptionPos.y + 2 * (fontSize + 75));   //Extra 75 pixels of space

    tmpText = quit.option.getString();

    quit.rect.left = topOptionPos.x;
    quit.rect.top = topOptionPos.y + 2 * (fontSize + 75);       //Multiply (fontsize + 75) by however many menu options come before it.
    quit.rect.width = (tmpText.length() * fontSize / 1.8);            //Adjust rectangles width based on the text
    quit.rect.height = fontSize;
}

int Menu::processChoice(sf::RenderWindow& window)
{

    sf::Event event;
    while(selection == 0)
    {

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                //Not working for some reason
            case sf::Event::Closed:
              //  window.Close();
                selection = 3;
                return 3;
                break;

            case sf::Event::MouseMoved:

                //First Menu Choice
                if(play.rect.contains(event.mouseMove.x, event.mouseMove.y))
                {
                    play.option.setColor(selectedColor);

                }
                else
                {
                    play.option.setColor(unselectedColor);
                }

                //Second Menu Choice
                if(options.rect.contains(event.mouseMove.x, event.mouseMove.y))
                {
                    options.option.setColor(selectedColor);

                }
                else
                {
                    options.option.setColor(unselectedColor);
                }

                //Third Menu Choice
                if(quit.rect.contains(event.mouseMove.x, event.mouseMove.y))
                {
                    quit.option.setColor(selectedColor);

                }
                else
                {
                    quit.option.setColor(unselectedColor);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if(play.rect.contains(sf::Mouse::getPosition(window)) && event.mouseButton.button == sf::Mouse::Left)
                {
                    selection = 1;
                    return selection;
                }

                if(options.rect.contains(sf::Mouse::getPosition(window)) && event.mouseButton.button == sf::Mouse::Left)
                {
                    selection = 2;
                    return selection;
                }

                if(quit.rect.contains(sf::Mouse::getPosition(window)) && event.mouseButton.button == sf::Mouse::Left)
                {
                    selection = 3;
                    return selection;
                }
                break;

            case sf::Event::Resized:
            {
                //Minimum window size
                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                // Minimum size
                if(size.x < 800)
                    size.x = 800;
                if(size.y < 600)
                    size.y = 600;
                window.setSize(static_cast<sf::Vector2u>(size));

                //Reset the view of the window
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

                //Resize the background image
                backgroundImage.setScale(static_cast<float>(event.size.width) / 1920, static_cast<float>(event.size.height) / 1080);
                backgroundImage.setOrigin(0, 0);

                //Adjust selection rectangles
                fixRectangles(float(event.size.width), event.size.height);
                break;
            }
            default:
                break;
            }
        }
        Show(window);
    }//end while
    return 3;
}

void Menu::fixRectangles(float width, float height)
{

    topOptionPos.x = width / 1.5;                     //Smaller numbers puts text further the the right. Larger goes left.
    topOptionPos.y = height / 2;

    play.option.setOrigin(0, 0);
    play.option.setPosition(topOptionPos.x, topOptionPos.y);
    play.rect.left = topOptionPos.x;
    play.rect.top = topOptionPos.y;

    options.option.setOrigin(0, 0);
    options.option.setPosition(topOptionPos.x, topOptionPos.y + fontSize + 75);
    options.rect.left = topOptionPos.x;
    options.rect.top = topOptionPos.y + (fontSize + 75);

    quit.option.setOrigin(0, 0);
    quit.option.setPosition(topOptionPos.x, topOptionPos.y + 2 * (fontSize + 75));
    quit.rect.left = topOptionPos.x;
    quit.rect.top = topOptionPos.y + 2 * (fontSize + 75);
}

void Menu::Show(sf::RenderWindow& window)
{
    window.clear();

    window.draw(backgroundImage);

    //Menu Choices
    window.draw(play.option);
    window.draw(options.option);
    window.draw(quit.option);

    window.display();
}
