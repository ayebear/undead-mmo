// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>
#include "clientnetwork.h"
#include "inputbox.h"
#include "textitemlist.h"
#include "gameobjects.h"

using namespace std;


// This class handles the client-side stuff for the chat like graphics and input
class Chat: public sf::Drawable
{
    public:
        Chat();

        void setUp(sf::FloatRect, GameObjects&);

        void setInput(bool);
        bool getInput();
        void toggleInput();
        void processInput(sf::Keyboard::Key);
        void processTextEntered(sf::Uint32);

        void messageHistoryUp();
        void messageHistoryDown();

        const string parseMessage();
        void printMessage(const string&, const sf::Color& color = sf::Color::White);

        void setUsername(const string&);
        bool connectToServer(const string&);
        void loginToServer(const string&);

        void handleScrolling(sf::Event&, sf::RenderWindow&);

        void update();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void setFont(sf::Font*);

        void clearMessage();
        void parseCommand(const string&);
        void saveCurrentMessage();
        void addToHistory(const string&);
        void showHelp(const string&);

        // Constants
        static const unsigned short maxMessages;
        static const short textSize;
        static const float oldMsgAge;
        static const float maxMsgAge;
        static const unsigned short maxMsgHistory;
        static const sf::Color cmdOutColor;
        static const map<string,string> help;

        // Dependencies
        ClientNetwork* netManager;

        // Visible objects
        TextItemList messageBox; // stores the current messages on the screen
        InputBox currentMsg;

        // Message history
        deque <string> msgHistory; // stores the history of your own messages
        int msgHistoryPos; // current location in message history deque

        // Other variables
        bool input;
        sf::Vector2f mainPos;
        sf::Vector2f chatSize;
        string username;
};

#endif
