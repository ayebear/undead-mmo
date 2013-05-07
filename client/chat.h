// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <deque>
#include <SFML/Graphics.hpp>

using namespace std;

struct TimedMsg
{
    TimedMsg(const sf::Text& t): text(t) {}
    sf::Text text;
    sf::Clock age;
};

// This class handles the client-side stuff for the chat like graphics and input

class Chat: public sf::Drawable
{
    public:
        Chat();
        void SetFont(sf::Font*);
        void SetInput(bool);
        bool GetInput();
        void ToggleInput();
        void ProcessInput(sf::Keyboard::Key);
        void SetPosition(float, float);
        void AddChar(char);
        void Backspace();
        void Delete();
        void MoveCursorLeft();
        void MoveCursorRight();
        void Home();
        void End();
        void RestartCursorTimer();
        const string ParseMessage();
        void PrintMessage(const string&, const sf::Color& color = sf::Color::White);
        void MessageHistoryUp();
        void MessageHistoryDown();
        void SetUsername(const string&);
        void ConnectToServer(const string&);
        void Update();
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:
        void FixMessagePositions();
        void FixInputPositions();
        void FixCursorPosition();
        void FixAllPositions();
        void ClearMessage();
        //void SendMessage(const string&);
        void ParseCommand(const string&);
        void SaveCurrentMessage();
        void AddToHistory(const string&);
        void ShowHelp(const string&);

        static const ushort maxMessages;
        static const short textSize;
        static const float cursorBlinkRate;
        static const float oldMsgAge;
        static const float maxMsgAge;
        static const ushort maxMsgHistory;
        static const sf::Color cmdOutColor;
        static const map<string,string> help;

        sf::Font* font;
        bool input;
        bool showCursor;
        sf::Vector2f mainPos;
        deque <TimedMsg> msgList;
        sf::Text currentMsg;
        deque <string> msgHistory; // stores the history of your own messages
        int msgHistoryPos; // current location in message history deque
        sf::RectangleShape cursor;
        sf::Clock cursorTimer;
        string username;
        sf::Text usernameText;
        int cursorPos;
};

#endif
