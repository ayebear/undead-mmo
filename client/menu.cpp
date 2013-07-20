// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "menu.h"
#include <sstream>

Menu::Menu()
{

}


Menu::~Menu()
{
    clearButtons();
}

void Menu::setUpMenu(const std::string& backgroundFile, short fontSize, sf::Vector2f topButtonPosition, GameObjects& theObjects)
{
    objects = &theObjects;

    windowSize.x = objects->window.getSize().x;
    windowSize.y = objects->window.getSize().y;
    menuView.reset(sf::FloatRect(0, 0, windowSize.x, windowSize.y));

    buttonWidthFactor = windowSize.x / topButtonPosition.x;
    buttonHeightFactor = windowSize.y / topButtonPosition.y;

    //Load background and font
    if (!bgTexture.loadFromFile(backgroundFile))
        exit(104);

    buttonFont = &objects->fontBold;

    bgImageSize = bgTexture.getSize();

    bgTexture.setSmooth(true);
    bgSprite.setTexture(bgTexture);

    //Button settings
    buttonFontSize = fontSize;
    topButtonPos.x = topButtonPosition.x;
    topButtonPos.y = topButtonPosition.y;


    //scale take a factor amount so  newSize/oldSize.
    //Resize image to fit the window.  Move the origin because scaling the image moves it.
    bgSprite.setScale(windowSize.x / bgImageSize.x, windowSize.y / bgImageSize.y);
    bgSprite.setOrigin(0, 0);

    buttonUnselectedColor = sf::Color::White;
    buttonSelectedColor = sf::Color::Yellow;
    selection = 1;
    selectionMade = false;

}

void Menu::setBackground(const std::string& backgroundFile)
{
     //Load background and font
    if (!bgTexture.loadFromFile(backgroundFile))
        exit(105);

    bgTexture.setSmooth(true);
    bgSprite.setTexture(bgTexture);
}

void Menu::setFont(const std::string& fontFile)
{
     if (!buttonFont->loadFromFile(fontFile))
        exit(305);
}

void Menu::setFontSize(short fontSize)
{
    buttonFontSize = fontSize;
}

void Menu::setTopButtonLocation(sf::Vector2f topButtonPosition)
{
    topButtonPos.x = topButtonPosition.x;
    topButtonPos.y = topButtonPosition.y;
}

void Menu::setButtonColors(sf::Color unselectedColor, sf::Color selectedColor)
{
    buttonUnselectedColor = unselectedColor;
    buttonSelectedColor = selectedColor;
}

void Menu::handleMouseMovement(sf::Event& event)
{
    unsigned int i = 0;
    while(menuOptions.size() >  i)
    {
        //First Menu Choice
        if(menuOptions[i]->rect.contains(event.mouseMove.x, event.mouseMove.y))
            selection = i + 1;
        i++;
    }
}

int Menu::handleMouseReleased(sf::Event& event)
{
    unsigned int i = 0;
    while(i < menuOptions.size())
    {
        if(menuOptions[i]->rect.contains(sf::Mouse::getPosition(objects->window)) && event.mouseButton.button == sf::Mouse::Left)
        {
            selection = i + 1;
            return selection;
        }
        i++;
    }
    return 0;

}

int Menu::handleKeyPressed(sf::Event& event)
{
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

    case sf::Keyboard::Key::F1:
    {
        //Get the current system time.
        time_t currTime = time(0);
        std::string fileName = "data/screenshots/";
        std::stringstream ss;
        ss << currTime;

        //Add the time.png to the end of the file name and save it.
        fileName += ss.str() + ".png";
        sf::Image scrShot = objects->window.capture();
        scrShot.saveToFile(fileName);
        break;
    }

    default:
        break;
    }
    return 0;
}

void Menu::handleResize(sf::Event& event)
{
    sf::Vector2f windowSize;
    windowSize.x = objects->window.getSize().x;
    windowSize.y = objects->window.getSize().y;
    menuView.setSize(windowSize);
    //Resize the background image
    bgSprite.setScale(windowSize.x / bgImageSize.x, windowSize.y / bgImageSize.y);
    bgSprite.setOrigin(0, 0);


    //Adjust selection rectangles
    fixRectangles(float(event.size.width), event.size.height);
}

void Menu::updateMenu()
{
    for(int i = 0; i < (int)menuOptions.size(); i++)
    {
        if(selection == i + 1)
            menuOptions[i]->buttonName.setColor(buttonSelectedColor);
        else
            menuOptions[i]->buttonName.setColor(buttonUnselectedColor);
    }
}

void Menu::clearButtons()
{
    for(auto& menuItem: menuOptions)
        delete menuItem;

    //Make sure there is nothing in the vector.
    menuOptions.clear();
}

/*
    Function to fix menu items when resizing occurs
*/
void Menu::fixRectangles(float width, float height)
{

    if(buttonWidthFactor != 0)
        topButtonPos.x = width / buttonWidthFactor;        //Smaller numbers puts text further the the right. Larger goes left.

    if(buttonHeightFactor != 0)
        topButtonPos.y = height / buttonHeightFactor;

    for(unsigned int i = 0; i < menuOptions.size(); i++)
    {
        menuOptions[i]->buttonName.setOrigin(0,0);
        menuOptions[i]->buttonName.setPosition(topButtonPos.x, topButtonPos.y + (i * (buttonFontSize + 75)));
        menuOptions[i]->rect.left = topButtonPos.x;
        menuOptions[i]->rect.top = topButtonPos.y + i * (buttonFontSize + buttonPadding);
    }

}

void Menu::addMenuButton(const std::string& itemName)
{
    std::string tmpText;

    int i = menuOptions.size();

    MenuButton* menuItem = new MenuButton();

    menuItem->buttonName.setString(itemName);
    menuItem->buttonName.setCharacterSize(buttonFontSize);
    menuItem->buttonName.setColor(buttonUnselectedColor);
    menuItem->buttonName.setFont(*buttonFont);
    menuItem->buttonName.setPosition(topButtonPos.x, topButtonPos.y + i * (buttonFontSize + buttonPadding));   //Extra pixels of space

    tmpText = menuItem->buttonName.getString();

    menuItem->rect.left = topButtonPos.x;
    menuItem->rect.top = topButtonPos.y + i * (buttonFontSize + buttonPadding);       //Multiply (fontsize + 75) by however many menu options come before it.
    menuItem->rect.width = (tmpText.length() * buttonFontSize / 1.8);            //Adjust rectangles width based on the text
    menuItem->rect.height = buttonFontSize;

    menuOptions.push_back(menuItem);
}

void Menu::addMenuButton(const std::string& itemName, sf::Vector2f position)
{
    std::string tmpText;

    MenuButton* menuItem = new MenuButton();

    menuItem->buttonName.setString(itemName);
    menuItem->buttonName.setCharacterSize(buttonFontSize);
    menuItem->buttonName.setColor(buttonUnselectedColor);
    menuItem->buttonName.setFont(*buttonFont);
    menuItem->buttonName.setPosition(position);   //Extra pixels of space

    tmpText = menuItem->buttonName.getString();

    menuItem->rect.left = position.x;
    menuItem->rect.top = position.y;       //Multiply (fontsize + 75) by however many menu options come before it.
    menuItem->rect.width = (tmpText.length() * buttonFontSize / 1.8);            //Adjust rectangles width based on the text
    menuItem->rect.height = buttonFontSize;

    menuOptions.push_back(menuItem);
}

void Menu::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.setView(menuView);
    // Draw the background
    window.draw(bgSprite);

    // Draw menu choices
    for(auto& i: menuOptions)
        window.draw(i->buttonName);


}
