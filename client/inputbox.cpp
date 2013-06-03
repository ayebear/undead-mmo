#include "inputbox.h"

const char InputBox::pwdChar = '*';

InputBox::InputBox()
{
    input = false;
    pwdMode = false;
    SetTextSize(16);
    SetColor(sf::Color::White);
}

void InputBox::ProcessInput(sf::Keyboard::Key keyCode)
{
    switch (keyCode)
    {
        case sf::Keyboard::BackSpace:
            Backspace();
            break;
        case sf::Keyboard::Delete:
            Delete();
            break;
        case sf::Keyboard::Left:
            Left();
            break;
        case sf::Keyboard::Right:
            Right();
            break;
        case sf::Keyboard::Home:
            Home();
            break;
        case sf::Keyboard::End:
            End();
            break;
        default:
            break;
    }
}

void InputBox::UpdateCursor()
{
    if (input)
        cursor.Update();
}

void InputBox::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(text);
    if (input)
        window.draw(cursor);
}

void InputBox::SetFont(sf::Font* theFont)
{
    font = theFont;
    text.setFont(*font);
}

void InputBox::SetPosition(float x, float y)
{
    text.setPosition(x, y);
    UpdateCursorPos();
}

void InputBox::SetPasswordMode(bool mode)
{
    pwdMode = mode;
}

void InputBox::SetShowRectangle(bool mode)
{
    showRect = mode;
}

void InputBox::SetTextSize(unsigned int size)
{
    text.setCharacterSize(size);
}

void InputBox::SetColor(sf::Color col)
{
    text.setColor(col);
    cursor.rect.setFillColor(col);
}

void InputBox::SetString(const string& str)
{
    textStr = str;
    pwdStr.clear();
    for (unsigned int x = 0; x < textStr.size(); x++)
        pwdStr += pwdChar;
    UpdateText();
    UpdateCursorPos();
}

void InputBox::SetInput(bool mode)
{
    input = mode;
    if (input)
        cursor.RestartTimer();
}

void InputBox::UpdateCursorPos()
{
    auto charPos = text.findCharacterPos(cursor.pos);
    charPos.x += 1;
    charPos.y += 2;
    cursor.rect.setPosition(charPos);
}

const string& InputBox::GetString()
{
    return textStr;
}

void InputBox::Clear()
{
    SetString("");
}

void InputBox::ResetCursor()
{
    cursor.pos = -1;
    UpdateCursorPos();
}

// This is called while the player is typing
void InputBox::AddChar(char c)
{
    cursor.RestartTimer();
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
    UpdateText();
    UpdateCursorPos();
}

// This is called when backspace is pressed
void InputBox::Backspace()
{
    cursor.RestartTimer();
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
        UpdateText();
        UpdateCursorPos();
    }
}

void InputBox::Delete()
{
    cursor.RestartTimer();
    if (!textStr.empty() && cursor.pos != -1 && cursor.pos < (int)textStr.size())
    {
	    textStr.erase(textStr.begin() + cursor.pos);
	    pwdStr.pop_back();
        UpdateText();
        UpdateCursorPos();
    }
}

void InputBox::UpdateText()
{
    if (pwdMode)
        text.setString(pwdStr);
    else
        text.setString(textStr);
}

void InputBox::Left()
{
	cursor.RestartTimer();
	if (cursor.pos == -1)
	{
		cursor.pos = text.getString().getSize() - 1;
		UpdateCursorPos();
    }
	else if (cursor.pos != 0)
	{
		cursor.pos--;
		UpdateCursorPos();
	}
}

void InputBox::Right()
{
	cursor.RestartTimer();
	if (cursor.pos != -1)
	{
		cursor.pos++;
		if (cursor.pos >= (int)textStr.size())
			cursor.pos = -1;
		UpdateCursorPos();
	}
}

void InputBox::Home()
{
    cursor.RestartTimer();
    cursor.pos = 0;
    UpdateCursorPos();
}

void InputBox::End()
{
    cursor.RestartTimer();
    cursor.pos = -1;
    UpdateCursorPos();
}
