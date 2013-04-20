// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "chat.h"
#include "../shared/packet.h"

const ushort Chat::maxMessages = 10;
const short Chat::textSize = 16;
const float Chat::cursorBlinkRate = 0.3;
const float Chat::oldMsgAge = 50;
const float Chat::maxMsgAge = 60;
const ushort Chat::maxMsgHistory = 100;
const sf::Color Chat::cmdOutColor = sf::Color::Cyan;
const map<string,string> Chat::help = {
    {"echo","Prints out the text after the command. Usage: /echo text goes here"},
    {"username","Sets your current username. Usage: /username YourUsername"},
    {"exit","Exits the game. Usage: /exit"}
};

Chat::Chat()
{
    if (!font.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(3);

    input = false;
    showCursor = false;
    mainPos.x = 0;
    mainPos.y = 0;
    msgHistoryPos = 0;

    usernameText.setFont(font);
    usernameText.setCharacterSize(textSize);
    usernameText.setColor(sf::Color::White);

    currentMsg.setFont(font);
    currentMsg.setCharacterSize(textSize);
    currentMsg.setColor(sf::Color::White);

    cursor.setSize(sf::Vector2f(2, textSize));
    cursor.setFillColor(sf::Color::White);

    SetUsername("Anonymous");
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
    FixAllPositions();
}

void Chat::FixMessagePositions()
{
    float y = mainPos.y + textSize * (maxMessages - msgList.size());
    for (auto& msg: msgList)
    {
        msg.text.setPosition(mainPos.x + 4, y);
        y += textSize;
    }
}

void Chat::FixInputPositions()
{
    usernameText.setPosition(mainPos.x + 4, mainPos.y + maxMessages * textSize);
    currentMsg.setPosition(usernameText.findCharacterPos(-1).x + mainPos.x + 4, mainPos.y + maxMessages * textSize);
}

void Chat::FixCursorPosition()
{
    auto lastCharPos = currentMsg.findCharacterPos(-1);
    lastCharPos.x += 1;
    lastCharPos.y += 2;
    cursor.setPosition(lastCharPos);
}

void Chat::FixAllPositions()
{
    FixMessagePositions();
    FixInputPositions();
    FixCursorPosition();
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

// This is called when enter is pressed
void Chat::ParseMessage(sf::TcpSocket& socket)
{
    string msgStr = currentMsg.getString();
    if (!msgStr.empty())
    {
        AddToHistory(msgStr);
        if (msgStr.front() == '/')
        {
            PrintMessage(msgStr, sf::Color::Red);
            ParseCommand(msgStr);
        }
        else
        {
            string fullStr = username + ": " + msgStr;
            SendMessage(fullStr, socket);
            PrintMessage(fullStr, sf::Color::Green);
        }
        ClearMessage();
    }
}

void Chat::SendMessage(const string& msg, sf::TcpSocket& socket)
{
    // Send the actual network packet
    sf::Packet packet;
    packet << Packet::ChatMessage << msg;
    socket.send(packet);
}

void Chat::ParseCommand(const string& msgStr)
{
    uint spacePos = msgStr.find(" ");
    string cmdStr = msgStr.substr(1, spacePos - 1);
    string content;
    if (spacePos != string::npos && spacePos < msgStr.size())
        content = msgStr.substr(spacePos + 1);
    // There are a few alternative commands, we could decide on which to use later, or just keep them all
    if (cmdStr == "test")
        PrintMessage("Command parser seems to be working!", cmdOutColor);
    else if (cmdStr == "echo" || cmdStr == "print")
        PrintMessage(content, cmdOutColor);
    else if (cmdStr == "username" || cmdStr == "set_username")
        SetUsername(content);
    else if (cmdStr == "help" || cmdStr == "?")
        ShowHelp(content);
    else if (cmdStr == "exit" || cmdStr == "quit")
        exit(0);
    else
        PrintMessage("Error: '" + cmdStr + "' is not a recognized command!", cmdOutColor);
}

void Chat::ShowHelp(const string& content)
{
    if (content.empty() || content == "help")
        PrintMessage("Shows how to use commands. Commands: echo, username, help, exit. Usage: /help command", cmdOutColor);
    else
    {
        auto i = help.find(content);
        if (i != help.end())
            PrintMessage(i->second, cmdOutColor);
        else
            PrintMessage("Hmm, not quite sure how to help you with that!", cmdOutColor);
    }
}

void Chat::PrintMessage(const string& msgStr, const sf::Color& color)
{
    if (!msgStr.empty())
    {
        sf::Text msgText(msgStr, font, textSize);
        msgText.setColor(color);
        msgList.push_back(TimedMsg(msgText));
        if (msgList.size() > maxMessages)
            msgList.pop_front();
        FixMessagePositions();
    }
}

void Chat::ClearMessage()
{
    currentMsg.setString("");
    FixCursorPosition();
}

void Chat::MessageHistoryUp()
{
    SaveCurrentMessage();
    msgHistoryPos--;
    if (msgHistoryPos < 0)
        msgHistoryPos = 0;
    else
    {
        currentMsg.setString(msgHistory[msgHistoryPos]);
        FixCursorPosition();
    }
}

void Chat::MessageHistoryDown()
{
    SaveCurrentMessage();
    msgHistoryPos++;
    if (msgHistoryPos >= (int)msgHistory.size())
        msgHistoryPos = msgHistory.size() - 1;
    else
    {
        currentMsg.setString(msgHistory[msgHistoryPos]);
        FixCursorPosition();
    }
}

void Chat::AddToHistory(const string& msgStr)
{
    // If the last element is blank, remove it, we don't want a stray blank string saved
    if (!msgHistory.empty() && msgHistory.back().empty())
        msgHistory.pop_back();

    // Add the message to history
    msgHistory.push_back(msgStr);

    // If the size has exceeded the max, remove the first element
    if (msgHistory.size() > maxMsgHistory)
        msgHistory.pop_front();

    // Set the position to the last element + 1 (so the code will know to append a new message)
    msgHistoryPos = msgHistory.size();
}

void Chat::SaveCurrentMessage()
{
    string msgStr = currentMsg.getString();
    if (msgHistoryPos >= (int)msgHistory.size()) // Check if you are typing a new message which is not saved yet
        msgHistory.push_back(msgStr); // Append it to the list
    else // You are editing an already added message
        msgHistory[msgHistoryPos] = msgStr; // Overwrite the message instead of appending a new one
}

void Chat::SetUsername(const string& str)
{
    if (!str.empty())
    {
        username = str;
        usernameText.setString(username + ":");
        FixInputPositions();
        PrintMessage("Username successfully set to '" + username + "'.", cmdOutColor);
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
    window.draw(usernameText);
    window.draw(currentMsg);
    if (input && showCursor)
        window.draw(cursor);
}
