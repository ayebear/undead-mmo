// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "chat.h"
#include <sstream>
#include "packet.h"
#include "gameobjects.h"

const unsigned short Chat::maxMessages = 10;
const short Chat::textSize = 16;
const unsigned short Chat::maxMsgHistory = 100;

const sf::Color Chat::Colors::normal = sf::Color::White;
const sf::Color Chat::Colors::privateMsg = sf::Color::Green;
const sf::Color Chat::Colors::server = sf::Color::Yellow;
const sf::Color Chat::Colors::commandEntered = sf::Color::Red;
const sf::Color Chat::Colors::commandOutput = sf::Color::Cyan;

const std::map<std::string, std::string> Chat::help = {
    {"echo", "Prints out the text after the command. Usage: /echo text goes here"},
    {"exit", "Exits the game. Usage: /exit"},
    {"pm", "Sends a private message to a player. Usage: /pm username message"},
};

Chat::Chat()
{
    packetBuilder = nullptr;
    focus = false;
    mainPos.x = 0;
    mainPos.y = 0;
    msgHistoryPos = 0;

    setUsername("Anonymous");
}

void Chat::setUp(sf::FloatRect sizeFactor, GameObjects& objects)
{
    mainPos.x = sizeFactor.left * objects.windowSize.x;
    mainPos.y = sizeFactor.top * objects.windowSize.y;
    chatSize.x = sizeFactor.width * objects.windowSize.x;
    chatSize.y = sizeFactor.height * objects.windowSize.y;

    //Input box tall enough to fit one line of text, and as wide as the messagebox
    //float inputBoxHeight = textSize;
    //float inputBoxWidth = chatSize.x;

    objects.client.registerCallback(Packet::ChatMessage, std::bind(&Chat::handleChatMessage, this, std::placeholders::_1));

    messageBox.setupList(objects.window, sizeFactor, objects.fontBold, textSize, false, false);

    //render window, font size, font, width of box, x Pos, Y Pos,
    currentMsg.setUp(16, objects.fontBold, mainPos.x + 3, mainPos.y + chatSize.y + 3, chatSize.x, false);

    packetBuilder = &objects.packetBuilder;
}

void Chat::setUsername(const std::string& str)
{
    if (!str.empty())
        username = str;
}

void Chat::setInput(bool in)
{
    focus = in;
    currentMsg.setInput(focus);
    messageBox.toggleBackground();
}

bool Chat::getInput()
{
    return focus;
}

void Chat::toggleInput()
{
    setInput(!focus);
}

void Chat::processInput(sf::Keyboard::Key keyCode)
{
    if (focus)
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

void Chat::clear()
{
    messageBox.clearList();
}

// This is called when enter is pressed
const std::string Chat::parseMessage()
{
    std::string msgStr = currentMsg.getString();
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
            std::string fullStr = username + ": " + msgStr;
            if (packetBuilder)
                packetBuilder->sendChatMessage(msgStr);
            //printMessage(fullStr, Colors::normal);
        }
        clearMessage();
    }
    return msgStr;
}

// We could also have server-side commands!
// These will need to be executed using a different character or a special command in here...
void Chat::parseCommand(const std::string& msgStr)
{
    auto spacePos = msgStr.find(" ");
    std::string cmdStr = msgStr.substr(1, spacePos - 1);
    std::string content;
    if (spacePos != std::string::npos && spacePos < msgStr.size())
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

void Chat::showHelp(const std::string& content)
{
    if (content.empty() || content == "help")
    {
        std::string commands;
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

void Chat::sendPrivateMessage(const std::string& content)
{
    auto spacePos = content.find(" ");
    if (spacePos != std::string::npos && spacePos < content.size())
    {
        std::string usernameStr = content.substr(0, spacePos);
        std::string msgStr = content.substr(spacePos + 1);
        if (!msgStr.empty() && packetBuilder)
            packetBuilder->sendChatMessage(msgStr, usernameStr);
    }
}

void Chat::printMessage(const std::string& msgStr, const sf::Color& color)
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

void Chat::addToHistory(const std::string& msgStr)
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
    std::string msgStr = currentMsg.getString();
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
    if (!currentMsg.isActive())
    {
        currentMsg.handleMouseClicked(event, window);

        if (currentMsg.isActive())
        {
            focus = true;
            messageBox.toggleBackground();
        }
    }

    if (focus)
        messageBox.handleMouseClicked(event, window);
}

void Chat::update()
{
    currentMsg.updateCursor();
}

void Chat::handleChatMessage(sf::Packet& packet)
{
    int subType;
    std::string msg;
    if (packet >> subType >> msg)
    {
        if (subType == Packet::Chat::Private)
            printMessage(msg, Colors::privateMsg);
        else if (subType == Packet::Chat::Public)
            printMessage(msg, Colors::normal);
        else if (subType == Packet::Chat::Server)
            printMessage(msg, Colors::server);
    }
}

void Chat::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(messageBox);

    window.setView(window.getDefaultView());
    window.draw(currentMsg);
}
