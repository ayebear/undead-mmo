// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "menu.h"

Menu::Menu(sf::RenderWindow& screen, sf::VideoMode videoMode)
{
    // TODO: Make a font class which loads all of the fonts, and pass a reference around to it
    if (!font.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(Errors::Font);

    if (!bgTexture.loadFromFile("data/images/ui/MenuBackground.png"))
        exit(Errors::Graphics);
    bgTexture.setSmooth(true);
    bgSprite.setTexture(bgTexture);

//scale take a factor amount so  newSize/oldSize.
//The background image I made is 1920 x 1080, so resize it for windowed users.  Move the origin because scaling the image moves it.
    bgSprite.setScale(static_cast<float>(videoMode.width) / 1920, static_cast<float>(videoMode.height) / 1080);
    bgSprite.setOrigin(0, 0);

    view.reset(sf::FloatRect(0, 0, videoMode.width, videoMode.height));

    topOptionPos.x = videoMode.width / 1.5;                     //Smaller numbers puts text further the the right. Larger goes left.
    topOptionPos.y = videoMode.height / 2;

    fontSize = 32;

    unselectedColor = sf::Color::White;
    selectedColor = sf::Color::Yellow;
    selection = 1;
    selectionMade = false;

    //Set up menuOption structs
    addMenuItem("Play");
    addMenuItem("Quit");

}

Menu::~Menu()
{
    for(auto& menuItem: menuOptions)
        delete menuItem;
}

int Menu::processChoice(sf::RenderWindow& window)
{

    sf::Event event;
    while(!selectionMade)
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
            {
                uint i = 0;
                while(menuOptions.size() > i)
                {
                    //First Menu Choice
                    if(menuOptions[i]->rect.contains(event.mouseMove.x, event.mouseMove.y))
                        selection = i + 1;
                    i++;
                }

                break;
            }
            case sf::Event::MouseButtonReleased:
            {

                uint i = 0;
                while(menuOptions.size() > i)
                {
                    if(menuOptions[i]->rect.contains(sf::Mouse::getPosition(window)) && event.mouseButton.button == sf::Mouse::Left)
                    {
                        selection = i + 1;
                        return selection;
                    }
                    i++;
                }

                break;
            }
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Return:
                    return selection;
                    break;

                case sf::Keyboard::Down:
                    if(selection < (int)menuOptions.size())
                        selection++;
                    else
                        selection = 1;
                    break;

                case sf::Keyboard::Up:
                    if(selection > 1)
                        selection--;
                    else
                        selection = menuOptions.size();
                    break;
                default:
                    break;
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
                bgSprite.setScale(static_cast<float>(event.size.width) / 1920, static_cast<float>(event.size.height) / 1080);
                bgSprite.setOrigin(0, 0);

                //Adjust selection rectangles
                fixRectangles(float(event.size.width), event.size.height);
                break;
            }
            default:
                break;
            }

            for(int i = 0; i < (int)menuOptions.size(); i++)
            {
                if(selection == i + 1)
                    menuOptions[i]->option.setColor(selectedColor);
                else
                    menuOptions[i]->option.setColor(unselectedColor);
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

    for(uint i = 0; i < menuOptions.size(); i++)
    {
        menuOptions[i]->option.setOrigin(0,0);
        menuOptions[i]->option.setPosition(topOptionPos.x, topOptionPos.y + (i * (fontSize + 75)));
        menuOptions[i]->rect.left = topOptionPos.x;
        menuOptions[i]->rect.top = topOptionPos.y + i * (fontSize + 75);
    }

}

void Menu::addMenuItem(string itemName)
{
    int i = menuOptions.size();

    MenuChoice* menuItem = new MenuChoice();

    menuItem->option.setString(itemName);
    menuItem->option.setCharacterSize(fontSize);
    menuItem->option.setColor(unselectedColor);
    menuItem->option.setFont(font);
    menuItem->option.setPosition(topOptionPos.x, topOptionPos.y + i * (fontSize + 75));   //Extra 75 pixels of space

    tmpText = menuItem->option.getString();

    menuItem->rect.left = topOptionPos.x;
    menuItem->rect.top = topOptionPos.y + i * (fontSize + 75);       //Multiply (fontsize + 75) by however many menu options come before it.
    menuItem->rect.width = (tmpText.length() * fontSize / 1.8);            //Adjust rectangles width based on the text
    menuItem->rect.height = fontSize;

    menuOptions.push_back(menuItem);
}

void Menu::Show(sf::RenderWindow& window)
{
    window.clear();

    // Draw the background
    window.draw(bgSprite);

    // Draw menu choices
    for(auto& i: menuOptions)
        window.draw(i->option);

    window.display();
}