#include "inputbox.h"

const char InputBox::pwdChar = '*';

InputBox::InputBox()
{
    input = false;
    pwdMode = false;
    setTextSize(16);
    setColor(sf::Color::White);
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

void InputBox::updateCursor()
{
    if (input)
        cursor.update();
}

void InputBox::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(text);
    if (input)
        window.draw(cursor);
}

void InputBox::setFont(sf::Font* theFont)
{
    font = theFont;
    text.setFont(*font);
}

void InputBox::setPosition(float x, float y)
{
    text.setPosition(x, y);
    updateCursorPos();
}

void InputBox::setPasswordMode(bool mode)
{
    pwdMode = mode;
}

void InputBox::setShowRectangle(bool mode)
{
    showRect = mode;
}

void InputBox::setTextSize(unsigned int size)
{
    text.setCharacterSize(size);
}

void InputBox::setColor(sf::Color col)
{
    text.setColor(col);
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
    auto charPos = text.findCharacterPos(cursor.pos);
    charPos.x += 1;
    charPos.y += 2;
    cursor.rect.setPosition(charPos);
}

const string& InputBox::getString()
{
    return textStr;
}

void InputBox::clear()
{
    setString("");
}

void InputBox::resetCursor()
{
    cursor.pos = -1;
    updateCursorPos();
}

// This is called while the player is typing
void InputBox::addChar(char c)
{
    cursor.restartTimer();
    if (textStr.empty() || cursor.pos == -1)
    {
	    textStr += c;
	    cursor.pos = -1;
	}
	else
	{
		textStr.insert(textStr.begin() + cursor.pos, c);
		cursor.pos++;
	}
	pwdStr += pwdChar;
    updateText();
    updateCursorPos();
}

// This is called when backspace is pressed
void InputBox::backspace()
{
    cursor.restartTimer();
    if (!textStr.empty() && cursor.pos != 0)
    {
    	if (cursor.pos == -1)
    		textStr.pop_back();
    	else
    	{
    		cursor.pos--;
	    	textStr.erase(textStr.begin() + cursor.pos);
	    }
	    pwdStr.pop_back();
        updateText();
        updateCursorPos();
    }
}

void InputBox::deleteChar()
{
    cursor.restartTimer();
    if (!textStr.empty() && cursor.pos != -1 && cursor.pos < (int)textStr.size())
    {
	    textStr.erase(textStr.begin() + cursor.pos);
	    pwdStr.pop_back();
        updateText();
        updateCursorPos();
    }
}

void InputBox::updateText()
{
    if (pwdMode)
        text.setString(pwdStr);
    else
        text.setString(textStr);
}

void InputBox::left()
{
	cursor.restartTimer();
	if (cursor.pos == -1)
	{
		cursor.pos = text.getString().getSize() - 1;
		updateCursorPos();
    }
	else if (cursor.pos != 0)
	{
		cursor.pos--;
		updateCursorPos();
	}
}

void InputBox::right()
{
	cursor.restartTimer();
	if (cursor.pos != -1)
	{
		cursor.pos++;
		if (cursor.pos >= (int)textStr.size())
			cursor.pos = -1;
		updateCursorPos();
	}
}

void InputBox::home()
{
    cursor.restartTimer();
    cursor.pos = 0;
    updateCursorPos();
}

void InputBox::end()
{
    cursor.restartTimer();
    cursor.pos = -1;
    updateCursorPos();
}
