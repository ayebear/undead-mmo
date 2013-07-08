// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>
#include "../shared/other.h"
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

        void SetNetManager(ClientNetwork*);
        void SetFont(sf::Font*);
        void setUp(sf::FloatRect, GameObjects&);

        void SetInput(bool);
        bool GetInput();
        void ToggleInput();
        void ProcessInput(sf::Keyboard::Key);
        void ProcessTextEntered(sf::Uint32);

        void MessageHistoryUp();
        void MessageHistoryDown();

        const string ParseMessage();
        void PrintMessage(const string&, const sf::Color& color = sf::Color::White);

        void SetUsername(const string&);
        bool ConnectToServer(const string&);
        void LoginToServer(const string&);

        void handleScrolling(sf::Event&, sf::RenderWindow&);

        void Update();
        void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void FixMessagePositions();
        void FixInputPositions();
        void FixAllPositions();
        void ClearMessage();
        //void SendMessage(const string&);
        void ParseCommand(const string&);
        void SaveCurrentMessage();
        void AddToHistory(const string&);
        void ShowHelp(const string&);

        // Constants
        static const ushort maxMessages;
        static const short textSize;
        static const float oldMsgAge;
        static const float maxMsgAge;
        static const ushort maxMsgHistory;
        static const sf::Color cmdOutColor;
        static const map<string,string> help;

        // Dependencies
        ClientNetwork* netManager;
        sf::Font* font;

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
        sf::Text usernameText;
};

#endif
