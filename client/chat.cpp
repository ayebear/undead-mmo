// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "chat.h"
#include <iostream>
#include <sstream>
#include "packet.h"

const unsigned short Chat::maxMessages = 10;
const short Chat::textSize = 16;
const float Chat::oldMsgAge = 50;
const float Chat::maxMsgAge = 60;
const unsigned short Chat::maxMsgHistory = 100;

const sf::Color Chat::Colors::normal = sf::Color::White;
const sf::Color Chat::Colors::privateMsg = sf::Color::Green;
const sf::Color Chat::Colors::server = sf::Color::Yellow;
const sf::Color Chat::Colors::commandEntered = sf::Color::Red;
const sf::Color Chat::Colors::commandOutput = sf::Color::Cyan;

const map<string,string> Chat::help = {
    {"echo", "Prints out the text after the command. Usage: /echo text goes here"},
    {"exit", "Exits the game. Usage: /exit"},
    {"pm", "Sends a private message to a player. Usage: /pm username message"},
};

Chat::Chat()
{
    netManager = nullptr;

    input = false;
    mainPos.x = 0;
    mainPos.y = 0;
    msgHistoryPos = 0;

    setUsername("Anonymous");
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

    netManager = &objects.netManager;

    messageBox.setupList(objects.window, sizeFactor, objects.fontBold, textSize, false, false);

    //render window, font size, font, width of box, x Pos, Y Pos,
    currentMsg.setUp(16, objects.fontBold, mainPos.x + 3, mainPos.y + chatSize.y + 3, chatSize.x, false);
}

void Chat::setUsername(const string& str)
{
    if (!str.empty())
    {
        username = str;
        cout << "Chat username set to: " << username << endl;
    }
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
    currentMsg.processTextEntered(text);
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
            printMessage(msgStr, Colors::commandEntered);
            parseCommand(msgStr);
        }
        else
        {
            string fullStr = username + ": " + msgStr;
            if (netManager != nullptr)
                netManager->sendChatMessage(msgStr);
            printMessage(fullStr, Colors::normal);
        }
        clearMessage();
    }
    return msgStr;
}

// We could also have server-side commands!
// These will need to be executed using a different character or a special command in here...
void Chat::parseCommand(const string& msgStr)
{
    auto spacePos = msgStr.find(" ");
    string cmdStr = msgStr.substr(1, spacePos - 1);
    string content;
    if (spacePos != string::npos && spacePos < msgStr.size())
        content = msgStr.substr(spacePos + 1);
    // TODO: Make a map of pointers to these functions
    if (cmdStr == "test")
        printMessage("Command parser seems to be working!", Colors::commandOutput);
    else if (cmdStr == "pm")
        sendPrivateMessage(content);
    else if (cmdStr == "echo")
        printMessage(content, Colors::commandOutput);
    else if (cmdStr == "help")
        showHelp(content);
    else if (cmdStr == "exit")
        exit(0);
    else
        printMessage("Error: '" + cmdStr + "' is not a recognized command!", Colors::commandOutput);
}

void Chat::showHelp(const string& content)
{
    if (content.empty() || content == "help")
    {
        string commands;
        for (auto& cmd: help)
            commands += cmd.first + ", ";
        printMessage("Shows how to use commands. Commands: " + commands + "help. Usage: /help command", Colors::commandOutput);
    }
    else
    {
        auto i = help.find(content);
        if (i != help.end())
            printMessage(i->second, Colors::commandOutput);
        else
            printMessage("Hmm, not quite sure how to help you with that!", Colors::commandOutput);
    }
}

void Chat::sendPrivateMessage(const string& content)
{
    auto spacePos = content.find(" ");
    if (spacePos != string::npos && spacePos < content.size())
    {
        string usernameStr = content.substr(0, spacePos);
        string msgStr = content.substr(spacePos + 1);
        if (!msgStr.empty() && netManager != nullptr)
        {
            netManager->sendChatMessage(msgStr, usernameStr);
            printMessage("Private message sent to server.", Colors::commandOutput);
        }
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

void Chat::handleScrolling(sf::Event& event, sf::RenderWindow& window)
{
    if(messageBox.isBackgroundVisible())
        messageBox.handleScrolling(event, window);
}

void Chat::handleMouseClicked(sf::Event& event, sf::RenderWindow& window)
{
    if(!currentMsg.isActive())
    {
        currentMsg.handleMouseClicked(event, window);

        if(currentMsg.isActive())
        {
            input = true;
            messageBox.toggleBackground();
        }
    }

}

void Chat::update()
{
    receiveMessages();
    currentMsg.updateCursor();
}

void Chat::receiveMessages()
{
    if (netManager != nullptr)
    {
        while (netManager->arePackets(Packet::ChatMessage))
        {
            int subType;
            string msg;
            if (netManager->getPacket(Packet::ChatMessage) >> subType >> msg)
            {
                if (subType == Packet::Chat::Private)
                    printMessage(msg, Colors::privateMsg);
                else if (subType == Packet::Chat::Public)
                    printMessage(msg, Colors::normal);
                else if (subType == Packet::Chat::Server)
                    printMessage(msg, Colors::server);
            }
            netManager->popPacket(Packet::ChatMessage);
        }
    }
}

void Chat::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(messageBox);

    window.setView(window.getDefaultView());
    window.draw(currentMsg);
}
