// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
    Change the transparency of the chat input and username text when focus is lost
    Maybe animate the PrintMessage command - would need to update the animation in Update()
*/

#include <sstream>
#include "chat.h"
#include "../shared/packet.h"

const ushort Chat::maxMessages = 10;
const short Chat::textSize = 16;
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
    mainPos.x = 0;
    mainPos.y = 0;
    msgHistoryPos = 0;

    usernameText.setCharacterSize(textSize);
    usernameText.setColor(sf::Color::White);

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
    currentMsg.SetFont(font);
}

void Chat::SetInput(bool in)
{
    input = in;
    currentMsg.SetInput(input);
}

bool Chat::GetInput()
{
    return input;
}

void Chat::ToggleInput()
{
    SetInput(!input);
}

void Chat::ProcessInput(sf::Keyboard::Key keyCode)
{
    if (input)
    {
        switch (keyCode)
        {
            case sf::Keyboard::Up:
                MessageHistoryUp();
                break;
            case sf::Keyboard::Down:
                MessageHistoryDown();
                break;
            default:
                currentMsg.ProcessInput(keyCode);
                break;
        }
    }
}

void Chat::ProcessTextEntered(sf::Uint32 text)
{
    if (input && text >= 32 && text <= 126)
        currentMsg.AddChar(static_cast<char>(text));
}

void Chat::setUp(float x, float y, float width, float height, GameObjects& objects)
{
    mainPos.x = x;
    mainPos.y = y;

    SetNetManager(&objects.netManager);
    messageBox.setupList(objects.window, sf::FloatRect(x, y, width, height), objects.fontBold, 16, false);

    currentMsg.SetPosition(x, y + height);
    currentMsg.SetFont(&objects.fontBold);

    usernameText.setPosition(mainPos.x + 4, y + height - 182);
    currentMsg.SetPosition(usernameText.findCharacterPos(-1).x + mainPos.x + 4, y + height - 182);
    FixAllPositions();
}

void Chat::FixMessagePositions()
{
   // messageBox.reposition(mainPos.x, mainPos.y);
}

void Chat::FixInputPositions()
{
    usernameText.setPosition(mainPos.x + 4, messageBox.getListDimensions().top + messageBox.getListDimensions().height);
   // currentMsg.SetPosition(usernameText.findCharacterPos(-1).x + mainPos.x + 4, messageBox.);
}

void Chat::FixAllPositions()
{
    FixMessagePositions();
    FixInputPositions();
}

// This is called when enter is pressed
const string Chat::ParseMessage()
{
    string msgStr = currentMsg.GetString();
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
    // TODO: Make a map of pointers to these functions
    if (cmdStr == "test")
        PrintMessage("Command parser seems to be working!", cmdOutColor);
    else if (cmdStr == "connect")
        ConnectToServer(content);
    else if (cmdStr == "login")
        LoginToServer(content);
    else if (cmdStr == "echo")
        PrintMessage(content, cmdOutColor);
    else if (cmdStr == "username")
        SetUsername(content);
    else if (cmdStr == "help")
        ShowHelp(content);
    else if (cmdStr == "exit")
        exit(0);
    else
        PrintMessage("Error: '" + cmdStr + "' is not a recognized command!", cmdOutColor);
}

bool Chat::ConnectToServer(const string& host)
{
    bool connected = false;
    if (host.empty() || host == "status")
        PrintMessage(netManager->GetStatusString());
    else
    {
        PrintMessage("Attempting a connection to '" + host + "'...", cmdOutColor);
        connected = netManager->ConnectToServer(host);
        if (connected)
            PrintMessage("Successfully connected to '" + host + "'.", cmdOutColor);
        else
            PrintMessage("Error: Could not connect to '" + host + "'.", cmdOutColor);
    }
    return connected;
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
            if (ConnectToServer(host))
            {
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
    messageBox.addTextItem(msgStr, color);
}

void Chat::ClearMessage()
{
    currentMsg.Clear();
}

void Chat::MessageHistoryUp()
{
    SaveCurrentMessage();
    msgHistoryPos--;
    if (msgHistoryPos < 0)
        msgHistoryPos = 0;
    else
    {
        currentMsg.SetString(msgHistory[msgHistoryPos]);
        currentMsg.ResetCursor();
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
        currentMsg.SetString(msgHistory[msgHistoryPos]);
        currentMsg.ResetCursor();
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
    string msgStr = currentMsg.GetString();
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
void Chat::handleScrolling(sf::Event& event, sf::RenderWindow& window)
{
    messageBox.handleScrolling(event, window);
}

void Chat::Update()
{
    currentMsg.UpdateCursor();
}

void Chat::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(messageBox);
    window.draw(usernameText);
    window.draw(currentMsg);
}


