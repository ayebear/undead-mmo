// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "chat.h"
#include "../shared/packet.h"

const ushort Chat::maxMessages = 10;
const short Chat::textSize = 16;
const float Chat::cursorBlinkRate = 0.3;
const float Chat::oldMsgAge = 10;
const float Chat::maxMsgAge = 20;

Chat::Chat()
{
    if (!font.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(3);

    input = false;
    showCursor = true;
    mainPos.x = 0;
    mainPos.y = 0;

    currentMsg.setFont(font);
    currentMsg.setCharacterSize(textSize);
    currentMsg.setColor(sf::Color::White);

    cursor.setSize(sf::Vector2f(2, textSize));
    cursor.setFillColor(sf::Color::White);
}

void Chat::SetInput(bool in)
{
    input = in;
}

bool Chat::GetInput()
{
    return input;
}

void Chat::ToggleInput()
{
    input = !input;
}

void Chat::SetPosition(float x, float y)
{
    mainPos.x = x;
    mainPos.y = y;
    FixPositions();
    FixCursorPosition();
}

void Chat::FixCursorPosition()
{
    auto lastCharPos = currentMsg.findCharacterPos(-1);
    lastCharPos.x += 1;
    lastCharPos.y += 2;
    cursor.setPosition(lastCharPos);
}

void Chat::FixPositions()
{
    float y = mainPos.y + textSize * (maxMessages - msgList.size());
    for (auto& msg: msgList)
    {
        msg.text.setPosition(mainPos.x + 4, y);
        y += textSize;
    }
    currentMsg.setPosition(mainPos.x + 4, mainPos.y + maxMessages * textSize);
}

// This is called while the player is typing
void Chat::AddChar(char c)
{
    string str = currentMsg.getString();
    str += c;
    currentMsg.setString(str);
    FixCursorPosition();
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
    FixCursorPosition();
}

// This is called when a new chat message is received from the server
void Chat::AddMessage(sf::Text msg)
{
    msgList.push_back(TimedMsg(msg));
    if (msgList.size() > maxMessages)
        msgList.pop_front();
    FixPositions();
}

// May need this overload
void Chat::AddMessage(string msg)
{
    sf::Text tmp(msg, font, textSize);
    AddMessage(tmp);
}

// This is called when enter is pressed
void Chat::SendMessage(sf::TcpSocket& socket)
{
    string str = currentMsg.getString();
    if (str != "")
    {
        // Send the actual network packet
        sf::Packet packet;
        packet << Packet::ChatMessage << str;
        socket.send(packet);

        // Update it visually
        AddMessage(str);
        msgList.back().text.setColor(sf::Color::Green);
        currentMsg.setString("");
        FixCursorPosition();
    }
}

void Chat::Update()
{
    for (auto& msg: msgList)
    {
        float msgAge = msg.age.getElapsedTime().asSeconds();
        if (msgAge >= maxMsgAge)
            msg.text.setColor(sf::Color::Transparent); // TODO: Actually erase it from the deque
        else if (msgAge >= oldMsgAge)
            msg.text.setColor(sf::Color(255, 255, 255, 128));
    }
    if (input && cursorTimer.getElapsedTime().asSeconds() >= cursorBlinkRate)
    {
        cursorTimer.restart();
        showCursor = !showCursor;
    }
}

void Chat::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    for (auto& msg: msgList)
        window.draw(msg.text);
    window.draw(currentMsg);
    if (input && showCursor)
        window.draw(cursor);
}
