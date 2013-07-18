#include "inputbox.h"
#include <iostream>

const char InputBox::pwdChar = '*';

InputBox::InputBox()
{
    input = false;
    pwdMode = false;
    setTextSize(16);
    setColor(sf::Color::White);
}

void InputBox::setUp(int fontSize, sf::Font& font, float xPos, float yPos, float widthInPx, bool passwordMode)
{
    setTextSize(fontSize);
    text.setFont(font);
    viewableText.setFont(font);
    font = font;
    setPosition(xPos, yPos);
    setPasswordMode(passwordMode);

    //The border that is seen around the inputbox
    inputBorder.setSize(sf::Vector2f(widthInPx, fontSize * 1.25));
    inputBorder.setPosition(xPos, yPos);
    inputBorder.setFillColor(sf::Color(15, 15, 15, 75));
    inputBorder.setOutlineColor(sf::Color(sf::Color(200, 200, 200, 100)));
    inputBorder.setOutlineThickness(2);

    //Invisible stationary box around the input Border
    collisionBox.left = inputBorder.getPosition().x;
    collisionBox.top = inputBorder.getPosition().y;
    collisionBox.width = inputBorder.getSize().x;
    collisionBox.height = inputBorder.getSize().y;

    //Box that adjusts its position based on what characters should be visible
    viewableChars.left = collisionBox.left;
    viewableChars.top = collisionBox.top;
    viewableChars.width = collisionBox.width;
    viewableChars.height = collisionBox.height;

    charsBeforeLeft.setFont(font);
    charsBeforeLeft.setCharacterSize(fontSize);
    charsBeforeLeft.setPosition(collisionBox.left, collisionBox.top);

    charsBeforeRight.setFont(font);
    charsBeforeRight.setCharacterSize(fontSize);
    charsBeforeRight.setPosition(collisionBox.left, collisionBox.top);

    leftCharIndex = 0;
    rightCharIndex = 0;

}

void InputBox::processInput(sf::Keyboard::Key keyCode)
{
    switch (keyCode)
    {
        case sf::Keyboard::BackSpace:
            backspace();
            break;
        case sf::Keyboard::Delete:
            deleteChar();
            break;
        case sf::Keyboard::Left:
            left();
            break;
        case sf::Keyboard::Right:
            right();
            break;
        case sf::Keyboard::Home:
            home();
            break;
        case sf::Keyboard::End:
            end();
            break;
        default:
            break;
    }
}

bool InputBox::isActive()
{
    return input;
}

void InputBox::processTextEntered(sf::Uint32 text)
{
 if (input && text >= 32 && text <= 126)
        addChar(static_cast<char>(text));
}

void InputBox::handleMouseClicked(sf::Event& event, sf::RenderWindow& window)
{
    //Get the mouse coordinates relative to the window to determine if it's inside of the item list
    sf::Vector2f mousePos;
    mousePos.x = sf::Mouse::getPosition(window).x;
    mousePos.y = sf::Mouse::getPosition(window).y;

    if(!input)
    {
        //If left mouse button is clicked while cursor is inside of the input box, allow input
        if(collisionBox.contains(mousePos) && event.mouseButton.button == sf::Mouse::Left)
        {
            setInput(true);
        }
    }

}

void InputBox::shiftViewCharsLeft()
{
    if(leftCharIndex > 0)
    {
        viewableChars.left -= 2 * text.getCharacterSize();
    }
}

void InputBox::shiftViewCharsRight()
{
    viewableChars.left += 2 * text.getCharacterSize();
}

void InputBox::updateCursor()
{
    if (input)
        cursor.update();
}

void InputBox::updateText()
{


    if (pwdMode)
        text.setString(pwdStr);
    else
        text.setString(textStr);

    std::string temp("");
    findLeftChar();
    findRightChar();
    //Generate a substring with only the visible characters

    temp = textStr.substr(leftCharIndex, rightCharIndex - leftCharIndex + 1);
    viewableText.setString(temp);


}

//Runs verry slow when a lot of characters are in the string. Need to improve this search function later on
void InputBox::findLeftChar()
{

    leftCharIndex = 0;
    std::string temp("");
    charsBeforeLeft.setString("");

    while(leftCharIndex < textStr.size() && charsBeforeLeft.getGlobalBounds().left + charsBeforeLeft.getGlobalBounds().width < viewableChars.left)
    {
        temp = textStr.substr(0, leftCharIndex);
        charsBeforeLeft.setString(temp);
        leftCharIndex++;
    }std::cout << "UPDATING TEXT\n";
}

//Runs verry slow when a lot of characters are in the string. Need to improve this search functionm later on
void InputBox::findRightChar()
{
    rightCharIndex = 0;
    std::string temp("");
    charsBeforeRight.setString("");

    while(rightCharIndex < textStr.size() && charsBeforeRight.getGlobalBounds().left +  charsBeforeRight.getGlobalBounds().width < viewableChars.left + viewableChars.width - fontSize)
    {
        temp = textStr.substr(0, rightCharIndex);
        charsBeforeRight.setString(temp);
        rightCharIndex++;
    }
}

void InputBox::setPosition(float x, float y)
{
    text.setPosition(x, y);
    viewableText.setPosition(x, y);
    updateCursorPos();
}

void InputBox::setPasswordMode(bool mode)
{
    pwdMode = mode;
}

void InputBox::setShowRectangle(bool mode)
{
    showBorder = mode;
}

void InputBox::setTextSize(unsigned int size)
{
    text.setCharacterSize(size);
    viewableText.setCharacterSize(size);
    fontSize = size;
}

void InputBox::setColor(sf::Color col)
{
    text.setColor(col);
    viewableText.setColor(col);
    cursor.rect.setFillColor(col);
}

void InputBox::setString(const string& str)
{
    textStr = str;
    pwdStr.clear();
    for (unsigned int x = 0; x < textStr.size(); x++)
        pwdStr += pwdChar;
    updateText();
    updateCursorPos();
}

void InputBox::setInput(bool mode)
{
    input = mode;
    if (input)
        cursor.restartTimer();
}

void InputBox::updateCursorPos()
{
    auto charPos = text.findCharacterPos(invisibleCursor.pos);
    auto cursorPos = viewableText.findCharacterPos(invisibleCursor.pos - leftCharIndex);
    charPos.x += 1;
    charPos.y += 2;
    invisibleCursor.rect.setPosition(charPos);
    cursor.rect.setPosition(cursorPos);
}

const string& InputBox::getString()
{
    return textStr;
}

void InputBox::clear()
{
    setString("");
    viewableChars.left = collisionBox.left;
    invisibleCursor.rect.setPosition(collisionBox.left + 1, collisionBox.top + 2);
}

void InputBox::resetCursor()
{
    invisibleCursor.pos = -1;
    cursor.pos = -1;
    updateCursorPos();
}

// This is called while the player is typing
void InputBox::addChar(char c)
{
    cursor.restartTimer();
    if (textStr.empty() || invisibleCursor.pos == -1)
    {
	    textStr += c;
	    invisibleCursor.pos = -1;
	}
	else
	{
		textStr.insert(textStr.begin() + invisibleCursor.pos, c);
		invisibleCursor.pos++;
	}

	if(invisibleCursor.rect.getPosition().x > viewableChars.left + viewableChars.width - fontSize)
        shiftViewCharsRight();

	pwdStr += pwdChar;
    updateText();
    updateCursorPos();
}

// This is called when backspace is pressed
void InputBox::backspace()
{
    cursor.restartTimer();
    if (!textStr.empty() && invisibleCursor.pos != 0)
    {
    	if (invisibleCursor.pos == -1)
    		textStr.pop_back();
    	else
    	{
    		invisibleCursor.pos--;
	    	textStr.erase(textStr.begin() + invisibleCursor.pos);
	    }
	    pwdStr.pop_back();

	    if(invisibleCursor.rect.getPosition().x < viewableChars.left + 2 * fontSize)
            shiftViewCharsLeft();

        updateText();
        updateCursorPos();
    }
}

void InputBox::deleteChar()
{
    cursor.restartTimer();
    if (!textStr.empty() && invisibleCursor.pos != -1 && invisibleCursor.pos < (int)textStr.size())
    {
	    textStr.erase(textStr.begin() + invisibleCursor.pos);
	    pwdStr.pop_back();
        updateText();
        updateCursorPos();
    }
}

void InputBox::left()
{
	cursor.restartTimer();
	if (invisibleCursor.pos == -1)
	{
		invisibleCursor.pos = text.getString().getSize() - 1;
		updateCursorPos();
    }
	else if (invisibleCursor.pos != 0)
	{
		invisibleCursor.pos--;
		updateCursorPos();
	}

    if(invisibleCursor.rect.getPosition().x < viewableChars.left + 2 * fontSize)
        shiftViewCharsLeft();
    updateText();

}

void InputBox::right()
{
	cursor.restartTimer();
	if (invisibleCursor.pos != -1)
	{
		invisibleCursor.pos++;
		if (invisibleCursor.pos >= (int)textStr.size())
			invisibleCursor.pos = -1;
		updateCursorPos();
	}
	if(invisibleCursor.rect.getPosition().x > viewableChars.left + viewableChars.width - fontSize)
        shiftViewCharsRight();
    updateText();
}

void InputBox::home()
{
    cursor.restartTimer();
    invisibleCursor.pos = 0;
    cursor.pos = 0;
    updateCursorPos();
}

void InputBox::end()
{
    cursor.restartTimer();
    invisibleCursor.pos = -1;
    cursor.pos = -1;
    updateCursorPos();
}

void InputBox::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(inputBorder);
    if (input)
    {
        window.draw(cursor);
        window.draw(viewableText);
    }
}
