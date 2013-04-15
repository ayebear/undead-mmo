#include "chat.h"

/*
For the chat input code:
Have a chat state that can be on/off. This should be turned on after a "T" pressed event is received, and turned off when "Escape" is pressed.
Use sf::Event::TextEntered to get the text entered, so we don't have to test for a bunch of different types of keys pressed.
*/

Chat::Chat()
{
    input = false;
    textSize = 32;
    mainPos.x = 50;
    mainPos.y = 50;
    if (!font.loadFromFile("data/fonts/Ubuntu-R.ttf"))
        exit(3);
    currentMsg.setFont(font);
    currentMsg.setCharacterSize(textSize);
    currentMsg.setColor(sf::Color::Red);
}

void Chat::SetInput(bool in)
{
    input = in;
}

bool Chat::GetInput()
{
    return input;
}

void Chat::SetPosition(float x, float y)
{
    mainPos.x = x;
    mainPos.y = y;
    FixPositions();
}

void Chat::FixPositions()
{
    float y = mainPos.y;
    for (auto& msg: msgList)
    {
        msg.setPosition(mainPos.x, y);
        y += textSize;
    }
    currentMsg.setPosition(mainPos.x, mainPos.y + maxMessages * textSize);
}

// This is called while the player is typing
void Chat::AddChar(char c)
{
    string str = currentMsg.getString();
    str += c;
    currentMsg.setString(str);
}

// This is called when backspace is pressed
void Chat::RemoveChar()
{
    string str = currentMsg.getString();
    if (!str.empty())
    {
        str.pop_back();
        currentMsg.setString(str);
    }
}

// This is called when a new chat message is received from the server
// TODO: This will need to re-position the messages too
void Chat::AddMessage(sf::Text msg)
{
    msg.setPosition(mainPos.x, mainPos.y + msgList.size() * textSize);
    msgList.push_back(msg);
    if (msgList.size() > maxMessages)
    {
        msgList.pop_front();
        FixPositions();
    }
}

// May need this overload
void Chat::AddMessage(string msg)
{
    sf::Text tmp(msg, font, textSize);
    AddMessage(tmp);
}

// This is called when enter is pressed
void Chat::SendMessage()
{
    if (currentMsg.getString() != "")
    {
        AddMessage(currentMsg.getString());
        msgList.back().setColor(sf::Color::Green);
        currentMsg.setString("");
    }
}

void Chat::Draw(sf::RenderWindow& window)
{
    for (auto& msg: msgList)
        window.draw(msg);
    window.draw(currentMsg);
}
