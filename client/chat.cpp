// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
    Change the transparency of the chat input and username text when focus is lost
    Program mouse controls for the chat
    Make some other subclasses like for the inputbox and/or cursor
    Maybe animate the PrintMessage command - would need to update the animation in Update()
*/

#include <sstream>
#include "chat.h"
#include "../shared/packet.h"
#include "../shared/other.h"

const ushort Chat::maxMessages = 10;
const short Chat::textSize = 16;
const float Chat::cursorBlinkRate = 0.3;
const float Chat::oldMsgAge = 50;
const float Chat::maxMsgAge = 60;
const ushort Chat::maxMsgHistory = 100;
const sf::Color Chat::cmdOutColor = sf::Color::Cyan;
const map<string,string> Chat::help = {
    {"echo", "Prints out the text after the command. Usage: /echo text goes here"},
    {"username", "Sets your current username. Usage: /username YourUsername"},
    {"exit", "Exits the game. Usage: /exit"},
    {"connect", "Connects to a server. Usage: /connect hostname"},
    {"login", "Connects and logs into a server. Usage: /login hostname username password"}
};

Chat::Chat()
{
    netManager = nullptr;
    font = nullptr;
    input = false;
    showCursor = false;
    mainPos.x = 0;
    mainPos.y = 0;
    msgHistoryPos = 0;
    cursorPos = 0;

    usernameText.setCharacterSize(textSize);
    usernameText.setColor(sf::Color::White);

    currentMsg.setCharacterSize(textSize);
    currentMsg.setColor(sf::Color::White);

    cursor.setSize(sf::Vector2f(2, textSize));
    cursor.setFillColor(sf::Color::White);

    SetUsername("Anonymous");
}

void Chat::SetNetManager(ClientNetwork* theNetMan)
{
    netManager = theNetMan;
}

void Chat::SetFont(sf::Font* theFont)
{
    font = theFont;
    usernameText.setFont(*font);
    currentMsg.setFont(*font);
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

void Chat::ProcessInput(sf::Keyboard::Key keyCode)
{
    if (input)
    {
        switch (keyCode)
        {
            case sf::Keyboard::BackSpace:
                Backspace();
                break;
            case sf::Keyboard::Delete:
                Delete();
                break;
            case sf::Keyboard::Up:
                MessageHistoryUp();
                break;
            case sf::Keyboard::Down:
                MessageHistoryDown();
                break;
            case sf::Keyboard::Left:
                MoveCursorLeft();
                break;
            case sf::Keyboard::Right:
                MoveCursorRight();
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
    auto charPos = currentMsg.findCharacterPos(cursorPos);
    charPos.x += 1;
    charPos.y += 2;
    cursor.setPosition(charPos);
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
    RestartCursorTimer();
    string str = currentMsg.getString();
    if (str.empty() || cursorPos == -1)
    {
	    str += c;
	    cursorPos = -1;
	}
	else
	{
		str.insert(str.begin() + cursorPos, c);
		cursorPos++;
	}
    currentMsg.setString(str);
    FixCursorPosition();
}

// This is called when backspace is pressed
void Chat::Backspace()
{
    RestartCursorTimer();
    string str = currentMsg.getString();
    if (!str.empty() && cursorPos != 0)
    {
    	if (cursorPos == -1)
    		str.pop_back();
    	else
    	{
    		cursorPos--;
	    	str.erase(str.begin() + cursorPos);
	    }
        currentMsg.setString(str);
        FixCursorPosition();
    }
}

void Chat::Delete()
{
    RestartCursorTimer();
    string str = currentMsg.getString();
    if (!str.empty() && cursorPos != -1 && cursorPos < (int)str.size())
    {
	    str.erase(str.begin() + cursorPos);
        currentMsg.setString(str);
        FixCursorPosition();
    }
}

void Chat::MoveCursorLeft()
{
	RestartCursorTimer();
	if (cursorPos == -1)
	{
		cursorPos = currentMsg.getString().getSize() - 1;
		FixCursorPosition();
    }
	else if (cursorPos != 0)
	{
		cursorPos--;
		FixCursorPosition();
	}
}

void Chat::MoveCursorRight()
{
	RestartCursorTimer();
	if (cursorPos != -1)
	{
		cursorPos++;
		string str = currentMsg.getString();
		if (cursorPos >= (int)str.size())
			cursorPos = -1;
		FixCursorPosition();
	}
}

void Chat::Home()
{
    RestartCursorTimer();
    cursorPos = 0;
    FixCursorPosition();
}

void Chat::End()
{
    RestartCursorTimer();
    cursorPos = -1;
    FixCursorPosition();
}

void Chat::RestartCursorTimer()
{
    cursorTimer.restart();
	showCursor = true;
}

// This is called when enter is pressed
const string Chat::ParseMessage()
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
            // TODO: Either have the chat class build a packet which Game passes to the Network class
            // or, pass a reference from Game to Chat of the Network class
            if (netManager == nullptr)
                exit(99);
            netManager->SendChatMessage(fullStr);
            PrintMessage(fullStr, sf::Color::Green);
        }
        ClearMessage();
    }
    return msgStr;
}

// We could also have server-side commands!
// These will need to be executed using a different character or a special command in here...
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
    else if (cmdStr == "connect")
        ConnectToServer(content);
    else if (cmdStr == "login")
        LoginToServer(content);
    else if (cmdStr == "echo" || cmdStr == "print")
        PrintMessage(content, cmdOutColor);
    else if (cmdStr == "username")
        SetUsername(content);
    else if (cmdStr == "help" || cmdStr == "?")
        ShowHelp(content);
    else if (cmdStr == "exit" || cmdStr == "quit")
        exit(Errors::Ok);
    else
        PrintMessage("Error: '" + cmdStr + "' is not a recognized command!", cmdOutColor);
}

void Chat::ConnectToServer(const string& host)
{
    if (host.empty() || host == "status")
        PrintMessage(netManager->GetStatusString());
    else
    {
        PrintMessage("Attempting a connection to '" + host + "'...", cmdOutColor);
        if (netManager->ConnectToServer(host))
            PrintMessage("Successfully connected to '" + host + "'.", cmdOutColor);
        else
            PrintMessage("Error: Could not connect to '" + host + "'.", cmdOutColor);
    }
}

void Chat::LoginToServer(const string& paramStr)
{
    if (!paramStr.empty())
    {
        istringstream params(paramStr);
        string host, username, password;
        params >> host >> username >> password;
        if (!host.empty() && !username.empty())
        {
            ConnectToServer(host);
            PrintMessage("Logging in...");
            int authStatus = netManager->Login(username, password);
            switch (authStatus)
            {
                case Packet::Auth::Successful:
                    PrintMessage("Logged in successfully!");
                    break;
                case Packet::Auth::InvalidUsername:
                    PrintMessage("Error: Invalid username.");
                    break;
                case Packet::Auth::InvalidPassword:
                    PrintMessage("Error: Invalid password.");
                    break;
                case Packet::Auth::AccountBanned:
                    PrintMessage("Error: Your account has been banned.");
                    break;
                default:
                    PrintMessage("Error: Unknown login failure.");
                    break;
            }
        }
    }
}

void Chat::ShowHelp(const string& content)
{
    if (content.empty() || content == "help")
    {
        string commands;
        for (auto& cmd: help)
            commands += cmd.first + ", ";
        PrintMessage("Shows how to use commands. Commands: " + commands + "help. Usage: /help command", cmdOutColor);
    }
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
    if (!msgStr.empty() && font != nullptr)
    {
        sf::Text msgText(msgStr, *font, textSize);
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
        cursorPos = -1;
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
        cursorPos = -1;
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


