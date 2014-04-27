// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "inputbox.h"
#include "textitemlist.h"

class GameObjects;
class PacketBuilder;

// TODO: Get rid of this from everywhere...
using namespace std;

/*
This class handles the client-side stuff for the chat like graphics and input
TODO:
    If we want to make this class more generic, it should use a callback or have some way
        of retreiving messages to be sent.
    Make the public interface a bit simpler.
*/
class Chat: public sf::Drawable
{
    public:
        Chat();

        void setUp(sf::FloatRect, GameObjects&);
        void setUsername(const string&);

        void setInput(bool);
        bool getInput();
        void toggleInput();
        void processInput(sf::Keyboard::Key);
        void processTextEntered(sf::Uint32);
        void clear();

        void messageHistoryUp();
        void messageHistoryDown();

        const string parseMessage();
        void printMessage(const string&, const sf::Color& color = sf::Color::White);

        void handleScrolling(sf::Event&, sf::RenderWindow&);
        void handleMouseClicked(sf::Event&, sf::RenderWindow&);

        void update();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void handleChatMessage(sf::Packet&);

        void setFont(sf::Font*);

        void clearMessage();
        void parseCommand(const string&);
        void saveCurrentMessage();
        void addToHistory(const string&);
        void showHelp(const string&);
        void sendPrivateMessage(const string&);

        // Constants
        static const unsigned short maxMessages;
        static const short textSize;
        static const float oldMsgAge;
        static const float maxMsgAge;
        static const unsigned short maxMsgHistory;
        static const map<string,string> help;

        struct Colors
        {
            static const sf::Color normal;
            static const sf::Color privateMsg;
            static const sf::Color server;
            static const sf::Color commandEntered;
            static const sf::Color commandOutput;
        };

        // Dependencies
        PacketBuilder* packetBuilder;

        // Visible objects
        TextItemList messageBox; // stores the current messages on the screen
        InputBox currentMsg;

        // Message history
        deque <string> msgHistory; // stores the history of your own messages
        int msgHistoryPos; // current location in message history deque

        // Other variables
        bool focus;
        sf::Vector2f mainPos;
        sf::Vector2f chatSize;
        string username;
};

#endif
