#include "statusbar.h"

StatusBar::StatusBar()
{
    statusBarName = "";
    minimumValue = 0;
    currentValue = 0;
    maximumValue = 100;
    empty = true;
    full = false;
    textAlwaysShown = false;
    valuesAsPercent = false;
    mousedOver = false;
}

StatusBar::~StatusBar()
{
    //dtor
}


void StatusBar::setUp(std::string& name, sf::Vector2f& pos, sf::Vector2f& size, int minValue, int currValue, int maxValue, int fillDirec, sf::Color& backColor, sf::Color& fillCol, sf::Font& valuesFont, bool textVisible, bool valsAsPercent)
{

    setPosition(pos);
    setSize(size);

    minimumValue = minValue;
    setMaximumValue(maxValue);
    setCurrentValue(currValue);

    setFillDirection(fillDirec);

    setBackgroundColor(backColor);
    setFillColor(fillCol);

    setStatusBarName(name);

    setValuesFont(valuesFont);
    values.setPosition((statusBar.left + statusBar.width) / 2, (statusBar.top + statusBar.height) / 2);
    values.setCharacterSize(16);

    setAlwaysVisible(textVisible);
    setTextAsPercent(valsAsPercent);

    updateBar();

}

void StatusBar::setCurrentValue(int currValue)
{
    if(currValue >= minimumValue && currValue <= maximumValue)
        currentValue = currValue;

}

void StatusBar::modifyCurrentValue(int amount)
{
    currentValue += amount;

    if(currentValue < minimumValue)
        currentValue = minimumValue;

    if(currentValue > maximumValue)
        currentValue = maximumValue;

    updateBar();
}

void StatusBar::setMaximumValue(int maxValue)
{
    if(maxValue > minimumValue)
        maximumValue = maxValue;
}

void StatusBar::modifyMaximumValue(int amount)
{
    if(maximumValue + amount > minimumValue)
        maximumValue += amount;
}

void StatusBar::setFillDirection(int fillDirec)
{
    fillDirection = fillDirec;

    if(fillDirection > 4)
        fillDirection = 4;
    else if(fillDirection < 1)
        fillDirection = 1;
}

void StatusBar::setPosition(sf::Vector2f& pos)
{
    xPos = pos.x;
    yPos = pos.y;

    statusBar.left = xPos;
    statusBar.top = yPos;
    backgroundBar.setPosition(xPos, yPos);

}

void StatusBar::setPosition(float x, float y)
{
    xPos = x;
    yPos = y;

    statusBar.left = xPos;
    statusBar.top = yPos;
    backgroundBar.setPosition(xPos, yPos);
}

void StatusBar::setSize(sf::Vector2f& size)
{
    barWidth = size.x;
    barHeight = size.y;

    statusBar.width = barWidth;
    statusBar.height = barHeight;

    backgroundBar.setSize(sf::Vector2f(barWidth, barHeight));
}

void StatusBar::setSize(float x, float y)
{
    barWidth = x;
    barHeight = y;

    statusBar.width = barWidth;
    statusBar.height = barHeight;

     backgroundBar.setSize(sf::Vector2f(barWidth, barHeight));
}

void StatusBar::setBackgroundColor(sf::Color& backColor)
{
    backgroundColor = backColor;
    backgroundBar.setFillColor(backgroundColor);
}

void StatusBar::setFillColor(sf::Color& fillCol)
{
    fillColor = fillCol;
    currentValueBar.setFillColor(fillColor);
}

void StatusBar::updateBar()
{
    float sizeFactor = 0;
    float percent = 0;

    if(maximumValue > 0)
        sizeFactor = float(currentValue) / maximumValue;

    percent = sizeFactor * 100;

    //Convert the current and maximum values to a string
    std::string value("");
    std::ostringstream temp;

    if(!valuesAsPercent)
        temp << statusBarName << ": " << currentValue << " / " << maximumValue;
    else
        temp << statusBarName << ": " << percent << "%";
    value = temp.str();
    values.setString(value);



    //std::cout << "Size Factor: " << sizeFactor << std::endl;

    //Fills Up
    if(fillDirection == 1)
    {
        currentValueBar.setSize(sf::Vector2f(statusBar.width , statusBar.height * sizeFactor));
        currentValueBar.setPosition(statusBar.left, (statusBar.top + statusBar.height) - currentValueBar.getSize().y);
        //values.rotate(90);
        values.setPosition(statusBar.left + (statusBar.width / 2) - (values.getGlobalBounds().width / 2) , statusBar.top + (statusBar.height / 2) - (values.getGlobalBounds().height / 2));


    }

    //Fills Right
    else if(fillDirection == 2)
    {
        currentValueBar.setSize(sf::Vector2f(statusBar.width * sizeFactor, statusBar.height));
        currentValueBar.setPosition(statusBar.left, statusBar.top);
        values.setPosition(statusBar.left + (statusBar.width / 2) - (values.getGlobalBounds().width / 2) , statusBar.top + (statusBar.height / 2) - (values.getGlobalBounds().height / 2));
    }

    //Fills Down
    else if(fillDirection == 3)
    {
        currentValueBar.setSize(sf::Vector2f(statusBar.width , statusBar.height * sizeFactor));
        currentValueBar.setPosition(statusBar.left, statusBar.top);
        //values.rotate(90);
        values.setPosition(statusBar.left + (statusBar.width / 2) - (values.getGlobalBounds().width / 2) , statusBar.top + (statusBar.height / 2) - (values.getGlobalBounds().height / 2));

    }

    //Fills Left
    else
    {
        currentValueBar.setSize(sf::Vector2f(statusBar.width * sizeFactor, statusBar.height));
        currentValueBar.setPosition((statusBar.left  + statusBar.width) - currentValueBar.getSize().x, statusBar.top);
        values.setPosition(statusBar.left + (statusBar.width / 2) - (values.getGlobalBounds().width / 2) , statusBar.top + (statusBar.height / 2) - (values.getGlobalBounds().height / 2));
    }


    //std::cout << "Background size: x - " << backgroundBar.getSize().x << "\ny - " << backgroundBar.getSize().y << std::endl;
    //std::cout << "Fill size: x - " << currentValueBar.getSize().x << "\ny - " << currentValueBar.getSize().y << std::endl;
    //std::cout << "Status bar size: x - " << statusBar.width << "\ny - " << statusBar.height << std::endl;

    if(currentValue <= minimumValue)
        empty = true;

    if(currentValue >= maximumValue)
        full = true;
}

void StatusBar::setValuesFont(sf::Font& valuesFont)
{
    values.setFont(valuesFont);
}

void StatusBar::setStatusBarName(std::string& name)
{
    statusBarName = name;
}

bool StatusBar::isEmpty()
{
    return empty;
}

bool StatusBar::isFull()
{
    return full;
}


bool StatusBar::isMousedOver(sf::RenderWindow& window)
{
    sf::Vector2f mousePos;
    mousePos.x = sf::Mouse::getPosition(window).x;
    mousePos.y = sf::Mouse::getPosition(window).y;

    if(statusBar.contains(mousePos))
        mousedOver = true;
    else
        mousedOver = false;

    return mousedOver;
}

void StatusBar::setAlwaysVisible(bool textVisible)
{
    textAlwaysShown = textVisible;
}

void StatusBar::setTextAsPercent(bool valsAsPercent)
{
    valuesAsPercent = valsAsPercent;
}

void StatusBar::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(backgroundBar);
    window.draw(currentValueBar);

    if(textAlwaysShown)
        window.draw(values);
    else if(mousedOver)
        window.draw(values);
}
