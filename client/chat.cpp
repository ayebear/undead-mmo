// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
TODO:
    Change the transparency of the chat input and username text when focus is lost
    Maybe animate the PrintMessage command - would need to update the animation in update()
*/

#include <sstream>
#include "chat.h"
#include "packet.h"

const unsigned short Chat::maxMessages = 10;
const short Chat::textSize = 16;
const float Chat::oldMsgAge = 50;
const float Chat::maxMsgAge = 60;
const unsigned short Chat::maxMsgHistory = 100;
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

    setUsername("Anonymous");
}

void Chat::setNetManager(ClientNetwork* theNetMan)
{
    netManager = theNetMan;
}

void Chat::setFont(sf::Font* theFont)
{
    font = theFont;
    usernameText.setFont(*font);
    currentMsg.setFont(font);
}

void Chat::setInput(bool in)
{
    input = in;
    currentMsg.setInput(input);
    messageBox.toggleBackground();
}

bool Chat::getInput()
{
    return input;
}

void Chat::toggleInput()
{
    setInput(!input);
}

void Chat::processInput(sf::Keyboard::Key keyCode)
{
    if (input)
    {
        switch (keyCode)
        {
            case sf::Keyboard::Up:
                messageHistoryUp();
                break;
            case sf::Keyboard::Down:
                messageHistoryDown();
                break;
            default:
                currentMsg.processInput(keyCode);
                break;
        }
    }
}

void Chat::processTextEntered(sf::Uint32 text)
{
    if (input && text >= 32 && text <= 126)
        currentMsg.addChar(static_cast<char>(text));
}

void Chat::setUp(sf::FloatRect sizeFactor, GameObjects& objects)
{
    mainPos.x = sizeFactor.left * objects.window.getSize().x;
    mainPos.y = sizeFactor.top * objects.window.getSize().y;
    chatSize.x = sizeFactor.width * objects.window.getSize().x;
    chatSize.y = sizeFactor.height * objects.window.getSize().y;

    //Input box tall enough to fit one line of text, and as wide as the messagebox
    //float inputBoxHeight = textSize;
    //float inputBoxWidth = chatSize.x;

    setNetManager(&objects.netManager);
    messageBox.setupList(objects.window, sizeFactor, objects.fontBold, textSize, false, false);

    //render window, font size, font, width of box, x Pos, Y Pos,
    currentMsg.setUp(16, &objects.fontMono, mainPos.x, mainPos.y + chatSize.y + 3, chatSize.x, false);
}

// This is called when enter is pressed
const string Chat::parseMessage()
{
    string msgStr = currentMsg.getString();
    if (!msgStr.empty())
    {
        addToHistory(msgStr);
        if (msgStr.front() == '/')
        {
            printMessage(msgStr, sf::Color::Red);
            parseCommand(msgStr);
        }
        else
        {
            string fullStr = username + ": " + msgStr;
            if (netManager == nullptr)
                exit(99);
            netManager->sendChatMessage(fullStr);
            printMessage(fullStr, sf::Color::Green);
        }
        clearMessage();
    }
    return msgStr;
}

// We could also have server-side commands!
// These will need to be executed using a different character or a special command in here...
void Chat::parseCommand(const string& msgStr)
{
    unsigned int spacePos = msgStr.find(" ");
    string cmdStr = msgStr.substr(1, spacePos - 1);
    string content;
    if (spacePos != string::npos && spacePos < msgStr.size())
        content = msgStr.substr(spacePos + 1);
    // TODO: Make a map of pointers to these functions
    if (cmdStr == "test")
        printMessage("Command parser seems to be working!", cmdOutColor);
    else if (cmdStr == "connect")
        connectToServer(content);
    else if (cmdStr == "login")
        loginToServer(content);
    else if (cmdStr == "echo")
        printMessage(content, cmdOutColor);
    else if (cmdStr == "username")
        setUsername(content);
    else if (cmdStr == "help")
        showHelp(content);
    else if (cmdStr == "exit")
        exit(0);
    else
        printMessage("Error: '" + cmdStr + "' is not a recognized command!", cmdOutColor);
}

bool Chat::connectToServer(const string& host)
{
    bool connected = false;
    if (host.empty() || host == "status")
        printMessage(netManager->getStatusString());
    else
    {
        printMessage("Attempting a connection to '" + host + "'...", cmdOutColor);
        connected = netManager->connectToServer(host);
        if (connected)
            printMessage("Successfully connected to '" + host + "'.", cmdOutColor);
        else
            printMessage("Error: Could not connect to '" + host + "'.", cmdOutColor);
    }
    return connected;
}

void Chat::loginToServer(const string& paramStr)
{
    if (!paramStr.empty())
    {
        istringstream params(paramStr);
        string host, username, password;
        params >> host >> username >> password;
        if (!host.empty() && !username.empty())
        {
            if (connectToServer(host))
            {
                printMessage("Logging in...");
                int authStatus = netManager->login(username, password);
                switch (authStatus)
                {
                    case Packet::Auth::Successful:
                        printMessage("Logged in successfully!");
                        break;
                    case Packet::Auth::InvalidUsername:
                        printMessage("Error: Invalid username.");
                        break;
                    case Packet::Auth::InvalidPassword:
                        printMessage("Error: Invalid password.");
                        break;
                    case Packet::Auth::AccountBanned:
                        printMessage("Error: Your account has been banned.");
                        break;
                    default:
                        printMessage("Error: Unknown login failure.");
                        break;
                }
            }
        }
    }
}

void Chat::showHelp(const string& content)
{
    if (content.empty() || content == "help")
    {
        string commands;
        for (auto& cmd: help)
            commands += cmd.first + ", ";
        printMessage("Shows how to use commands. Commands: " + commands + "help. Usage: /help command", cmdOutColor);
    }
    else
    {
        auto i = help.find(content);
        if (i != help.end())
            printMessage(i->second, cmdOutColor);
        else
            printMessage("Hmm, not quite sure how to help you with that!", cmdOutColor);
    }
}

void Chat::printMessage(const string& msgStr, const sf::Color& color)
{
    messageBox.addTextItem(msgStr, color);
}

void Chat::clearMessage()
{
    currentMsg.clear();
}

void Chat::messageHistoryUp()
{
    saveCurrentMessage();
    msgHistoryPos--;
    if (msgHistoryPos < 0)
        msgHistoryPos = 0;
    else
    {
        currentMsg.setString(msgHistory[msgHistoryPos]);
        currentMsg.resetCursor();
    }
}

void Chat::messageHistoryDown()
{
    saveCurrentMessage();
    msgHistoryPos++;
    if (msgHistoryPos >= (int)msgHistory.size())
        msgHistoryPos = msgHistory.size() - 1;
    else
    {
        currentMsg.setString(msgHistory[msgHistoryPos]);
        currentMsg.resetCursor();
    }
}

void Chat::addToHistory(const string& msgStr)
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

void Chat::saveCurrentMessage()
{
    string msgStr = currentMsg.getString();
    if (msgHistoryPos >= (int)msgHistory.size()) // Check if you are typing a new message which is not saved yet
        msgHistory.push_back(msgStr); // Append it to the list
    else // You are editing an already added message
        msgHistory[msgHistoryPos] = msgStr; // Overwrite the message instead of appending a new one
}

void Chat::setUsername(const string& str)
{
    if (!str.empty())
    {
        username = str;
        usernameText.setString(username + ":");
        printMessage("Username successfully set to '" + username + "'.", cmdOutColor);
    }
}
void Chat::handleScrolling(sf::Event& event, sf::RenderWindow& window)
{
    if(messageBox.isBackgroundVisible())
        messageBox.handleScrolling(event, window);
}

void Chat::update()
{
    currentMsg.updateCursor();
}

void Chat::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(messageBox);

    window.setView(window.getDefaultView());
    window.draw(currentMsg);
}


