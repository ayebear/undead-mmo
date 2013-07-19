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


    leftCharIndex = 0;
    rightCharIndex = 0;


}

void InputBox::processInput(sf::Keyboard::Key keyCode)
{
    if(input)
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

    //If left mouse button is clicked while cursor is inside of the input box, allow input
    if(collisionBox.contains(mousePos) && event.mouseButton.button == sf::Mouse::Left)
    {
        setInput(true);
    }
    else
        setInput(false);


}

void InputBox::shiftViewCharsLeft()
{
    if(leftCharIndex > 0)
    {
        viewableChars.left -= text.getCharacterSize();
    }
}

void InputBox::shiftViewCharsRight()
{
    viewableChars.left += text.getCharacterSize();
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
/*

//Need to improve this algorithm to estimate starting points

int InputBox::optimizeSearchStart(float desiredPos, sf::Text& fullText)
{
    int textSize = fullText.getString().getSize();
    int start = (textSize) / 2;
    int oldStart = (textSize) / 2;


    //Find start position
    if (fullText.findCharacterPos(start).x > desiredPos)
    {
        while(fullText.findCharacterPos(start).x > desiredPos && start / 2 >= 0)
        {
            oldStart = start;
            start /= 2;
        }
        start = oldStart;
    }
    else if (fullText.findCharacterPos(start).x < desiredPos)
    {
        while (fullText.findCharacterPos(start).x < desiredPos && start < textSize)
        {std::cout << "OPTIMIZING\n";
            oldStart = start;
            start += ((textSize - 1) + start) / 2;
        }
        start = oldStart;
    }
    else
    {
        std::cout << "SETTING START TO 0\n";
        start = 0;
    }

    return start;
}*/
//Runs verry slow when a lot of characters are in the string. Need to improve this search function later on
void InputBox::findLeftChar()
{

    leftCharIndex = 0;

    while(leftCharIndex < textStr.size() && text.findCharacterPos(leftCharIndex).x < viewableChars.left)
    {
        leftCharIndex++;
    }


}

//Runs verry slow when a lot of characters are in the string. Need to improve this search functionm later on
void InputBox::findRightChar()
{
    rightCharIndex = 0;


    while(rightCharIndex < textStr.size() && text.findCharacterPos(rightCharIndex).x < viewableChars.left + viewableChars.width - fontSize)
    {
        rightCharIndex++;
    }
    std::cout << "RIGHT CHAR: " << rightCharIndex << endl;


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
    end();

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
    leftCharIndex = 0;
    rightCharIndex = 0;
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
    viewableChars.left = collisionBox.left;
    invisibleCursor.pos = 0;
    cursor.pos = 0;
    updateText();
    updateCursorPos();
}

void InputBox::end()
{
    cursor.restartTimer();
    invisibleCursor.pos = -1;
    cursor.pos = -1;
    viewableChars.left = text.getGlobalBounds().left + text.getGlobalBounds().width - viewableChars.width;
    updateText();
    updateCursorPos();
}

void InputBox::draw(sf::RenderTarget& window, sf::RenderStates states) const
{

    window.setView(window.getDefaultView());
    window.draw(inputBorder);
    if (input)
    {
        window.draw(cursor);

    }
	window.draw(viewableText);
}
